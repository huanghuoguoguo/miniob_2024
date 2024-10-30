//
// Created by glwuy on 24-10-29.
//

#include "view.h"

#include <common/log/log.h>
#include <sql/parser/expression_binder.h>
#include "storage/index/bplus_tree_index.h"
#include <limits.h>
#include <string.h>
#include <bitset>
#include "common/defs.h"
#include "common/lang/string.h"
#include "common/lang/span.h"
#include "common/lang/algorithm.h"
#include "common/log/log.h"
#include "common/global_context.h"
#include "storage/db/db.h"
#include "storage/buffer/disk_buffer_pool.h"
#include "storage/common/condition_filter.h"
#include "storage/common/meta_util.h"
#include "storage/index/bplus_tree_index.h"
#include "storage/index/index.h"
#include "storage/record/record_manager.h"
#include "storage/table/table.h"

#include <regex>
#include <sql/expr/expression.h>

#include "storage/trx/trx.h"
int sql_parse(const char *st, ParsedSqlResult *sql_result);


View::~View()
{
  // ~Table();
}

RC View::create_view(Db *db, const char *         path, const char *base_dir, int32_t table_id, const char *name,
    SelectStmt *         select_stmt, std::string sql)
{
  if (table_id < 0) {
    LOG_WARN("invalid table id. table_id=%d, table_name=%s", table_id, name);
    return RC::INVALID_ARGUMENT;
  }

  if (common::is_blank(name)) {
    LOG_WARN("Name cannot be empty");
    return RC::INVALID_ARGUMENT;
  }
  LOG_INFO("Begin to create view %s", name);
  // 使用 table_name.table记录一个表的元数据
  RC rc = RC::SUCCESS;
  // 判断表文件是否已经存在
  int fd = ::open(path, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
  if (fd < 0) {
    if (EEXIST == errno) {
      LOG_ERROR("Failed to create table file, it has been created. %s, EEXIST, %s", path, strerror(errno));
      return RC::SCHEMA_TABLE_EXIST;
    }
    LOG_ERROR("Create table file failed. filename=%s, errmsg=%d:%s", path, errno, strerror(errno));
    return RC::IOERR_OPEN;
  }
  close(fd);
  // 打开文件以写入
  std::ofstream ofs(path, std::ios::out | std::ios::app); // 以追加模式打开文件
  if (!ofs.is_open()) {
    return RC::IOERR_OPEN;
  }

  // 写入字符串到文件
  ofs << name << '\n' << table_id << '\n' << sql;
  if (ofs.fail()) {
    ofs.close();
    return RC::IOERR_OPEN;
  }

  ofs.close();
  this->table_id_          = table_id;
  this->sql                = sql;
  this->view_name_         = name;
  this->selectStmt         = select_stmt;
  this->binderContext_     = select_stmt->binder_context();
  std::set<Table *> tables = this->binderContext_->query_tables();
  for (auto &table : tables) {
    this->tables.insert(table);
  }
  rc = init();
  if (rc != RC::SUCCESS) {
    return rc;
  }

  // 将元数据保存在文件中。
  db_       = db;
  base_dir_ = base_dir;

  return rc;
}

SelectStmt *View::select_stmt()
{
  ParsedSqlResult parsed_sql_result;
  sql_parse(sql.c_str(), &parsed_sql_result);
  std::unique_ptr<ParsedSqlNode> sql_node        = std::move(parsed_sql_result.sql_nodes().front());
  SelectSqlNode *                select_sql_node = sql_node->create_view.select_sql_node;
  Stmt *                         stmt;
  SelectStmt::create(this->db_, *select_sql_node, stmt);
  return static_cast<SelectStmt *>(stmt);
}

RC View::open(Db *db, const char *meta_file, const char *base_dir)
{
  RC rc = RC::SUCCESS;
  if (meta_file == nullptr || strlen(meta_file) == 0) {
    std::cerr << "Invalid file path" << std::endl;
    return RC::INVALID_ARGUMENT;
  }

  std::fstream fs;
  string       meta_file_path = string(base_dir) + common::FILE_PATH_SPLIT_STR + meta_file;
  // 打开元数据文件
  fs.open(meta_file_path, std::ios::in | std::ios::binary);
  if (!fs.is_open()) {
    return RC::IOERR_OPEN;
  }
  string view_name;
  std::getline(fs, view_name);
  string id;
  std::getline(fs, id);
  string sql;
  // 读取字符串
  std::getline(fs, sql);
  if (fs.fail()) {
    fs.close();
    return RC::IOERR_OPEN;
  }

  fs.close();

  // 拿到stmt之后，绑定。
  this->table_id_          = std::stoi(id);
  this->db_                = db;
  this->sql                = sql;
  this->base_dir_          = base_dir;
  this->view_name_         = view_name;
  this->selectStmt         = static_cast<SelectStmt *>(this->select_stmt());
  this->binderContext_     = this->selectStmt->binder_context();
  std::set<Table *> tables = this->binderContext_->query_tables();
  for (auto &table : tables) {
    this->tables.insert(table);
  }
  rc = init();
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return rc;
}

RC View::init()
{
  RC rc = RC::SUCCESS;
  if (tables.size() == 1) {
    this->current_table                       = *tables.begin();
    this->table_meta_                         = &current_table->table_meta();
    const std::vector<FieldMeta> *field_metas = this->table_meta_->field_metas();
    for (auto &field_meta : *field_metas) {
      this->tuple_schemata_.emplace_back(this->view_name_.c_str(), field_meta.name(), "");
    }
    this->tuple_schemata_.erase(this->tuple_schemata_.begin());
  } else {
    // 将查询列拿出来作为新的table_meta
    std::vector<AttrInfoSqlNode>              spv;
    std::vector<std::unique_ptr<Expression>> &query_expressions = this->select_stmt()->query_expressions();
    for (auto &query_expression : query_expressions) {
      AttrInfoSqlNode attr_info_sql_node;
      attr_info_sql_node.type     = query_expression->value_type();
      attr_info_sql_node.name     = query_expression->name();
      attr_info_sql_node.length   = query_expression->value_length();
      attr_info_sql_node.nullable = true;
      spv.push_back(attr_info_sql_node);
      // TODO 有坑。
      this->tuple_schemata_.emplace_back(this->view_name_.c_str(), query_expression->name(), "");
    }
    std::span<AttrInfoSqlNode> attributes(spv);
    // 创建文件
    const vector<FieldMeta> *trx_fields = this->db_->trx_kit().trx_fields();
    if ((rc = table_meta_->init(this->table_id_,
             this->view_name_.c_str(),
             trx_fields,
             attributes,
             StorageFormat::ROW_FORMAT)) != RC::SUCCESS) {
      return rc; // delete table file
    }
  }
  return rc;
}

RC View::make_record(int value_num, const Value *values, Record &record)
{
  RC rc = RC::SUCCESS;
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  this->current_table->make_record(value_num, values, record);
  return rc;
}

RC View::insert_record(Record &record)
{
  RC rc = RC::SUCCESS;
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  this->current_table->insert_record(record);
  return rc;
}

RC View::delete_record(const Record &record)
{
  RC rc = RC::SUCCESS;
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  this->current_table->delete_record(record);
  return rc;
}

RC View::delete_record(const RID &rid)
{
  RC rc = RC::SUCCESS;
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  this->current_table->delete_record(rid);
  return rc;
}

RC View::update_record(const Record &record)
{
  RC rc = RC::SUCCESS;
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  this->current_table->update_record(record);
  return rc;
}

RC View::get_record(const RID &rid, Record &record)
{
  RC rc = RC::SUCCESS;
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  this->current_table->get_record(rid, record);
  return rc;
}

RC View::sync()
{
  return RC::SUCCESS;
}

RC View::recover_insert_record(Record &record)
{
  RC rc = RC::SUCCESS;
  // 假设有一个方法可以恢复插入的记录
  // 这里可以添加具体的恢复逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  this->current_table->recover_insert_record(record);
  return rc;
}

RC View::create_index(Trx *trx, vector<unique_ptr<Expression>> &column_expressions_, const char *index_name,
    bool                   is_unique)
{
  RC rc = RC::SUCCESS;
  // 创建索引的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  // 调用当前表的创建索引方法
  rc = this->current_table->create_index(trx, column_expressions_, index_name, is_unique);
  return rc;
}

RC View::get_record_scanner(RecordFileScanner &scanner, Trx *trx, ReadWriteMode mode)
{
  RC rc = RC::SUCCESS;
  // 获取记录扫描器的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->get_record_scanner(scanner, trx, mode);
  return rc;
}

RC View::get_chunk_scanner(ChunkFileScanner &scanner, Trx *trx, ReadWriteMode mode)
{
  RC rc = RC::SUCCESS;
  // 获取块扫描器的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->get_chunk_scanner(scanner, trx, mode);
  return rc;
}

RC View::write_text(int64_t &offset, int64_t length, const char *data) const
{
  RC rc = RC::SUCCESS;
  // 写入文本的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->write_text(offset, length, data);
  return rc;
}

RC View::read_text(int64_t offset, int64_t length, char *data) const
{
  RC rc = RC::SUCCESS;
  // 读取文本的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->read_text(offset, length, data);
  return rc;
}

RC View::visit_record(const RID &rid, function<bool(Record &)> visitor)
{
  RC rc = RC::SUCCESS;
  // 访问记录的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->visit_record(rid, visitor);
  return rc;
}

RC View::insert_entry_of_indexes(const char *record, const RID &rid)
{
  RC rc = RC::SUCCESS;
  // 插入索引条目的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->insert_entry_of_indexes(record, rid);
  return rc;
}

RC View::delete_entry_of_indexes(const char *record, const RID &rid, bool error_on_not_exists)
{
  RC rc = RC::SUCCESS;
  // 删除索引条目的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->delete_entry_of_indexes(record, rid, error_on_not_exists);
  return rc;
}

RC View::set_value_to_record(char *record_data, const Value &value, const FieldMeta *field)
{
  RC rc = RC::SUCCESS;
  // 设置记录值的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->set_value_to_record(record_data, value, field);
  return rc;
}


Index *View::find_index(const char *index_name) const
{
  // 查找索引的逻辑
  if (this->tables.size() > 1) {
    return nullptr; // 或者返回一个特定的错误索引
  }
  return this->current_table->find_index(index_name);
}

Index *View::find_index_by_field(const char *field_name) const
{
  // 根据字段查找索引的逻辑
  if (this->tables.size() > 1) {
    return nullptr;
  }
  return this->current_table->find_index_by_field(field_name);
}

Index *View::find_index_by_field(const std::vector<string> field_names) const
{
  // 根据字段名列表查找索引的逻辑
  if (this->tables.size() > 1) {
    return nullptr;
  }
  return this->current_table->find_index_by_field(field_names);
}

RC View::drop_all_index()
{
  RC rc = RC::SUCCESS;
  // 删除所有索引的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->drop_all_index();
  return rc;
}

RC View::init_record_handler(const char *base_dir)
{
  RC rc = RC::SUCCESS;
  // 删除所有索引的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->init_record_handler(base_dir);
  return rc;
}

RC View::init_text_handler(const char *base_dir)
{
  RC rc = RC::SUCCESS;
  // 删除所有索引的逻辑
  if (this->tables.size() > 1) {
    return RC::INVALID_ARGUMENT;
  }
  rc = this->current_table->init_text_handler(base_dir);
  return rc;
}