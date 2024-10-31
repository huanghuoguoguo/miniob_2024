/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Meiyi & Wangyunlai on 2021/5/13.
//

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

Table::~Table()
{
  if (record_handler_ != nullptr) {
    delete record_handler_;
    record_handler_ = nullptr;
  }

  if (data_buffer_pool_ != nullptr) {
    data_buffer_pool_->close_file();
    data_buffer_pool_ = nullptr;
  }

  if(text_buffer_pool_ != nullptr) {
    text_buffer_pool_->close_file();
    text_buffer_pool_ = nullptr;
  }

  for (vector<Index *>::iterator it = indexes_.begin(); it != indexes_.end(); ++it) {
    Index *index = *it;
    delete index;
  }
  indexes_.clear();

  LOG_INFO("Table has been closed: %s", name());
}

RC Table::create(Db *db, int32_t table_id, const char *path, const char *name, const char *base_dir,
    span<const AttrInfoSqlNode> attributes, StorageFormat storage_format)
{
  if (table_id < 0) {
    LOG_WARN("invalid table id. table_id=%d, table_name=%s", table_id, name);
    return RC::INVALID_ARGUMENT;
  }

  if (common::is_blank(name)) {
    LOG_WARN("Name cannot be empty");
    return RC::INVALID_ARGUMENT;
  }
  LOG_INFO("Begin to create table %s:%s", base_dir, name);

  if (attributes.size() == 0) {
    LOG_WARN("Invalid arguments. table_name=%s, attribute_count=%d", name, attributes.size());
    return RC::INVALID_ARGUMENT;
  }

  RC rc = RC::SUCCESS;

  // 使用 table_name.table记录一个表的元数据
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

  // 创建文件
  const vector<FieldMeta> *trx_fields = db->trx_kit().trx_fields();
  if ((rc = table_meta_.init(table_id, name, trx_fields, attributes, storage_format)) != RC::SUCCESS) {
    LOG_ERROR("Failed to init table meta. name:%s, ret:%d", name, rc);
    return rc;  // delete table file
  }

  fstream fs;
  fs.open(path, ios_base::out | ios_base::binary);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", path, strerror(errno));
    return RC::IOERR_OPEN;
  }

  // 记录元数据到文件中
  table_meta_.serialize(fs);
  fs.close();

  db_       = db;
  base_dir_ = base_dir;

  string             data_file = table_data_file(base_dir, name);
  BufferPoolManager &bpm       = db->buffer_pool_manager();
  rc                           = bpm.create_file(data_file.c_str());
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create disk buffer pool of data file. file name=%s", data_file.c_str());
    return rc;
  }

  rc = init_record_handler(base_dir);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to create table %s due to init record handler failed.", data_file.c_str());
    // don't need to remove the data_file
    return rc;
  }

  // 创建文件存放text 以及 高纬vector文件
  bool exist_text_feild = false;
  bool exist_vector_feild = false;
  for (const FieldMeta &field : *table_meta_.field_metas()) {
    if (AttrType::TEXTS == field.type()) {
      exist_text_feild = true;
      break;
    }
    if(AttrType::VECTORS == field.type() && field.is_high_dim()==true) {
      LOG_INFO("table.cpp vector size is high dimension: %s", field.is_high_dim() ? "true" : "false");
      exist_vector_feild = true;
      break;
    }
  }
  if (exist_text_feild) {
    std::string text_file = table_text_file(base_dir, name);
    rc = bpm.create_file(text_file.c_str());

    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to create disk buffer pool of text file. file name=%s", text_file.c_str());
      return rc;
    }
    rc = init_text_handler(base_dir);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to create table %s due to init text handler failed.", text_file.c_str());
      return rc;
    }
  }
  if(exist_vector_feild) {
    std::string vector_file = table_vector_file(base_dir, name);
    rc = bpm.create_file(vector_file.c_str());

    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to create disk buffer pool of vector file. file name=%s", vector_file.c_str());
      return rc;
    }
    rc = init_vector_handler(base_dir);

    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to create table %s due to init vector handler failed.", vector_file.c_str());
      return rc;
    }
  }

  base_dir_ = base_dir;
  LOG_INFO("Successfully create table %s:%s", base_dir, name);
  return rc;
}

RC Table::open(Db *db, const char *meta_file, const char *base_dir)
{
  // 加载元数据文件
  fstream fs;
  string  meta_file_path = string(base_dir) + common::FILE_PATH_SPLIT_STR + meta_file;
  fs.open(meta_file_path, ios_base::in | ios_base::binary);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open meta file for read. file name=%s, errmsg=%s", meta_file_path.c_str(), strerror(errno));
    return RC::IOERR_OPEN;
  }
  if (table_meta_.deserialize(fs) < 0) {
    LOG_ERROR("Failed to deserialize table meta. file name=%s", meta_file_path.c_str());
    fs.close();
    return RC::INTERNAL;
  }
  fs.close();

  db_       = db;
  base_dir_ = base_dir;

  // 加载数据文件
  RC rc = init_record_handler(base_dir);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open table %s due to init record handler failed.", base_dir);
    // don't need to remove the data_file
    return rc;
  }

  // 如果 text 文件存在，则加载text文件
  rc = init_text_handler(base_dir);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open table %s due to init text handler failed.", base_dir);
    return rc;
  }

  // 加载高纬度vector数据文件
  rc = init_vector_handler(base_dir);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open table %s due to init text handler failed.", base_dir);
    return rc;
  }


  const int index_num = table_meta_.index_num();
  for (int i = 0; i < index_num; i++) {
    const IndexMeta *index_meta = table_meta_.index(i);
    std::vector<const FieldMeta *>* field_list = new std::vector<const FieldMeta *>();
    for (std::string field_str : index_meta->field()) {
      const FieldMeta *field_meta = table_meta_.field(field_str.c_str());
      if (field_meta == nullptr) {
        LOG_ERROR("Found invalid index meta info which has a non-exists field. table=%s, index=%s, field=%s",
            name(),
            index_meta->name(),
            field_str.c_str());
        // skip cleanup
        //  do all cleanup action in destructive Table function
        return RC::INTERNAL;
      }
      field_list->emplace_back(field_meta);
    }

    BplusTreeIndex *index      = new BplusTreeIndex();
    string          index_file = table_index_file(base_dir, name(), index_meta->name());

    rc = index->open(this, index_file.c_str(), *index_meta, *field_list);
    if (rc != RC::SUCCESS) {
      delete index;
      LOG_ERROR("Failed to open index. table=%s, index=%s, file=%s, rc=%s",
                name(), index_meta->name(), index_file.c_str(), strrc(rc));
      // skip cleanup
      //  do all cleanup action in destructive Table function.
      return rc;
    }
    indexes_.push_back(index);
  }

  return rc;
}

RC Table::insert_record(Record &record)
{
  RC rc = RC::SUCCESS;
  rc    = record_handler_->insert_record(record.data(), table_meta_.record_size(), &record.rid());
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Insert record failed. table name=%s, rc=%s", table_meta_.name(), strrc(rc));
    return rc;
  }

  std::vector<Index *> temp_indexes;
  RC rc2 = RC::SUCCESS;
  for (Index *index : indexes_) {
    rc = index->insert_entry(record.data(), &record.rid());
    if (rc != RC::SUCCESS) {
      LOG_WARN("插入索引失败。");
      break;
    }
    temp_indexes.push_back(index);
  }

  if (rc != RC::SUCCESS) {
    // 插入索引失败，删除原来插入的索引
    for(Index *index : temp_indexes) {
      rc2 = index->delete_entry(record.data(), &record.rid());
      if (rc2 != RC::SUCCESS) {
        if (rc2 != RC::RECORD_INVALID_KEY) {
          LOG_WARN("插入索引失败，删除之前插入的索引失败。");
          break;
        }
      }
    }
  }
  if (rc != RC::SUCCESS) {  // 可能出现了键值重复
    rc2 = record_handler_->delete_record(&record.rid());
    if (rc2 != RC::SUCCESS) {
      LOG_PANIC("Failed to rollback record data when insert index entries failed. table name=%s, rc=%d:%s",
                name(), rc2, strrc(rc2));
    }
  }
  return rc;
}

RC Table::visit_record(const RID &rid, function<bool(Record &)> visitor)
{
  return record_handler_->visit_record(rid, visitor);
}

RC Table::get_record(const RID &rid, Record &record)
{
  RC rc = record_handler_->get_record(rid, record);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to visit record. rid=%s, table=%s, rc=%s", rid.to_string().c_str(), name(), strrc(rc));
    return rc;
  }

  return rc;
}

RC Table::recover_insert_record(Record &record)
{
  RC rc = RC::SUCCESS;
  rc    = record_handler_->recover_insert_record(record.data(), table_meta_.record_size(), record.rid());
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Insert record failed. table name=%s, rc=%s", table_meta_.name(), strrc(rc));
    return rc;
  }

  rc = insert_entry_of_indexes(record.data(), record.rid());
  if (rc != RC::SUCCESS) {  // 可能出现了键值重复
    RC rc2 = delete_entry_of_indexes(record.data(), record.rid(), false /*error_on_not_exists*/);
    if (rc2 != RC::SUCCESS) {
      LOG_ERROR("Failed to rollback index data when insert index entries failed. table name=%s, rc=%d:%s",
                name(), rc2, strrc(rc2));
    }
    rc2 = record_handler_->delete_record(&record.rid());
    if (rc2 != RC::SUCCESS) {
      LOG_PANIC("Failed to rollback record data when insert index entries failed. table name=%s, rc=%d:%s",
                name(), rc2, strrc(rc2));
    }
  }
  return rc;
}

const char *Table::name() const { return table_meta_.name(); }

const TableMeta &Table::table_meta() const { return table_meta_; }

RC Table::make_record(int value_num, const Value *values, Record &record)
{
  RC rc = RC::SUCCESS;
  // 检查字段类型是否一致
  if (value_num + table_meta_.sys_field_num() != table_meta_.field_num()) {
    LOG_WARN("Input values don't match the table's schema, table name:%s", table_meta_.name());
    return RC::SCHEMA_FIELD_MISSING;
  }

  const int normal_field_start_index = table_meta_.sys_field_num();
  // 复制所有字段的值
  int   record_size = table_meta_.record_size();
  char *record_data = (char *)malloc(record_size);
  std::bitset<32> null_list;
  memset(record_data, 0, record_size);

  for (int i = 0; i < value_num && OB_SUCC(rc); i++) {
    const FieldMeta *field = table_meta_.field(i + normal_field_start_index);
    const Value &    value = values[i];
    if (!field->nullable() && value.is_null()) {
      return RC::SCHEMA_FIELD_MISSING;
    }
    if (field->type() != value.attr_type() && !value.is_null()) {
      Value real_value;
      if (AttrType::TEXTS == field->type() && AttrType::CHARS == value.attr_type()) {
        rc = set_value_to_record(record_data, value, field);
      }else {
        rc = Value::cast_to(value, field->type(), real_value);
        if (OB_FAIL(rc)) {
          LOG_WARN("failed to cast value. table name:%s,field name:%s,value:%s ",
              table_meta_.name(), field->name(), value.to_string().c_str());
          return rc;
        }
      }
      // 妥协之举，字符串转为数字时，取开头的第一个得到的数字，如果没得到数字，认为是0。但是在字符串转数字时要进行插入时，如果不是纯数字，则插入失败。
      if(field->type() == AttrType::INTS || field->type() == AttrType::FLOATS) {
        auto isNumber = [](const std::string& str) {
          std::regex pattern("^[+-]?([0-9]*[.])?[0-9]+$");
          return std::regex_match(str, pattern);
        };
        if(!isNumber(value.to_string())) {
          return RC::INVALID_ARGUMENT;
        }
      }
      rc = set_value_to_record(record_data, real_value, field);
    } else if(value.is_null()) {
      // 将bitmap对应位置置为true。
      null_list.set(field->field_id());
    } else {
      rc = set_value_to_record(record_data, value, field);
    }
  }
  const FieldMeta *field = table_meta_.field("null_list");
  rc = set_value_to_record(record_data, Value(static_cast<int>(null_list.to_ulong())), field);

  if (OB_FAIL(rc)) {
    LOG_WARN("failed to make record. table name:%s", table_meta_.name());
    free(record_data);
    return rc;
  }

  record.set_data_owner(record_data, record_size);
  return RC::SUCCESS;
}

RC Table::set_value_to_record(char *record_data, const Value &value, const FieldMeta *field)
{
  if(value.is_null()) {
    return RC::SUCCESS;
  }
  size_t       copy_len = field->len();
  const size_t data_len = value.length();
  if (field->type() == AttrType::CHARS) {
    if (copy_len > data_len) {
      copy_len = data_len + 1;
    }
  }
  if (field->type() == AttrType::TEXTS) {
    // 对于TEXTS类型字段，将字符串插入到文件中，并将offset和length写入record
    // 目前是将TEXT的放入cell前 将其TYPE 设置为CHARS 所以这里 应该不会运行到
    int64_t position[2];  // position[0] 是 offset, position[1] 是 length
    position[0] = field->offset();
    position[1] = value.length();
    // 假设 `text_buffer_pool_` 是一个用于存储大文本的缓冲池
    text_buffer_pool_->append_data(position[0], position[1], value.data());
    // 将偏移量和长度写入record
    memcpy(record_data + field->offset(), position, 2 * sizeof(int64_t));
  }else if(field->type() == AttrType::VECTORS && field->is_high_dim()==true){
    // 对于高纬度Vector类型字段，解决思路和TEXTS类型类似
    //TODO 这里放入position的值可能有点问题
    int64_t position[2];  // position[0] 是 offset, position[1] 是 length
    position[0] = field->offset();
    position[1] = value.length();
    vector_buffer_pool_->append_data(position[0], position[1], value.data());
    memcpy(record_data + field->offset(), position, 2 * sizeof(int64_t));
  }else {
    memcpy(record_data + field->offset(), value.data(), copy_len);
  }
  return RC::SUCCESS;
}

RC Table::write_text(int64_t &offset, int64_t length, const char *data)const
{
  RC rc = RC::SUCCESS;
  rc = text_buffer_pool_->append_data(offset, length, data);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to append text into disk_buffer_pool, rc=%s", strrc(rc));
    offset = -1;
    length = -1;
  }
  return rc;
}

RC Table::read_text(int64_t offset, int64_t length, char *data) const
{
  RC rc = RC::SUCCESS;
  if (0 > offset || 0 > length) {
    LOG_ERROR("Invalid param: text offset %ld, length %ld", offset, length);
    return RC::INVALID_ARGUMENT;
  }

  rc = text_buffer_pool_->get_data(offset, length, data);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to get text from disk_buffer_pool, rc=%s", strrc(rc));
  }
  return rc;
}

// write_vector
RC Table::write_vector(int64_t &offset, int64_t length, const char *data)const
{
  RC rc = RC::SUCCESS;
  rc = vector_buffer_pool_->append_data(offset, length, data);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to append vector data into disk_buffer_pool, rc=%s", strrc(rc));
    offset = -1;
    length = -1;
  }
  return rc;
}
// read_vector
RC Table::read_vector(int64_t offset, int64_t length, char *data) const
{
  RC rc = RC::SUCCESS;
  if (0 > offset || 0 > length) {
    LOG_ERROR("Invalid param: vector offset %ld, length %ld", offset, length);
    return RC::INVALID_ARGUMENT;
  }

  rc = vector_buffer_pool_->get_data(offset, length, data);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to get vector from disk_buffer_pool, rc=%s", strrc(rc));
  }
  return rc;
}

RC Table::init_record_handler(const char *base_dir)
{
  string data_file = table_data_file(base_dir, table_meta_.name());

  BufferPoolManager &bpm = db_->buffer_pool_manager();
  RC                 rc  = bpm.open_file(db_->log_handler(), data_file.c_str(), data_buffer_pool_);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open disk buffer pool for file:%s. rc=%d:%s", data_file.c_str(), rc, strrc(rc));
    return rc;
  }

  record_handler_ = new RecordFileHandler(table_meta_.storage_format());

  rc = record_handler_->init(*data_buffer_pool_, db_->log_handler(), &table_meta_);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to init record handler. rc=%s", strrc(rc));
    data_buffer_pool_->close_file();
    data_buffer_pool_ = nullptr;
    delete record_handler_;
    record_handler_ = nullptr;
    return rc;
  }

  return rc;
}

RC Table::init_text_handler(const char *base_dir) {
  // 构建文本文件路径
  std::string text_file = table_text_file(base_dir, table_meta_.name());

  // 检查文本文件是否存在
  if (!std::filesystem::exists(text_file)) {  // C++17 文件存在性检查
    LOG_INFO("Text file %s not found. Skipping buffer pool initialization.", text_file.c_str());
    return RC::SUCCESS;  // 如果文件不存在，返回成功状态，跳过初始化
  }

  // 获取 BufferPoolManager 实例
  BufferPoolManager &bpm = db_->buffer_pool_manager();

  // 打开文件并关联到 text_buffer_pool_
  RC rc = bpm.open_file(db_->log_handler(), text_file.c_str(), text_buffer_pool_);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open disk buffer pool for file:%s. rc=%d:%s", text_file.c_str(), rc, strrc(rc));
    return rc;
  }

  return rc;
}

RC Table::init_vector_handler(const char *base_dir) {
  // 构建文本文件路径
  std::string vector_file = table_vector_file(base_dir, table_meta_.name());

  // 检查文本文件是否存在
  if (!std::filesystem::exists(vector_file)) {  // C++17 文件存在性检查
    LOG_INFO("Text file %s not found. Skipping buffer pool initialization.", vector_file.c_str());
    return RC::SUCCESS;  // 如果文件不存在，返回成功状态，跳过初始化
  }

  // 获取 BufferPoolManager 实例
  BufferPoolManager &bpm = db_->buffer_pool_manager();

  // 打开文件并关联到 vector_buffer_pool_
  RC rc = bpm.open_file(db_->log_handler(), vector_file.c_str(), vector_buffer_pool_);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open disk buffer pool for file:%s. rc=%d:%s", vector_file.c_str(), rc, strrc(rc));
    return rc;
  }

  return rc;
}

RC Table::get_record_scanner(RecordFileScanner &scanner, Trx *trx, ReadWriteMode mode)
{
  RC rc = scanner.open_scan(this, *data_buffer_pool_, trx, db_->log_handler(), mode, nullptr);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("failed to open scanner. rc=%s", strrc(rc));
  }
  return rc;
}

RC Table::get_chunk_scanner(ChunkFileScanner &scanner, Trx *trx, ReadWriteMode mode)
{
  RC rc = scanner.open_scan_chunk(this, *data_buffer_pool_, db_->log_handler(), mode);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("failed to open scanner. rc=%s", strrc(rc));
  }
  return rc;
}


RC Table::create_index(Trx *trx, vector<unique_ptr<Expression>> &column_expressions_, const char *index_name,
    bool                    is_unique)
{
  // 收集需要创建的索引的列信息。
  std::vector<const FieldMeta *> *field_meta = new vector<const FieldMeta*>();
  for (auto &field_meta_expression : column_expressions_) {
    FieldExpr *field_expr = static_cast<FieldExpr *>(field_meta_expression.release());
    const FieldMeta *       meta       = field_expr->field().meta();
    field_meta->emplace_back(new FieldMeta(*meta));
  }
  column_expressions_.clear();

  for (const FieldMeta *field : *field_meta) {
    if (nullptr == field) {
      LOG_INFO("Invalid input arguments, table name is %s, index_name is blank or attribute_name is blank", name());
      return RC::INVALID_ARGUMENT;
    }
  }

  IndexMeta new_index_meta;
  RC        rc = new_index_meta.init(index_name, *field_meta, is_unique);
  if (rc != RC::SUCCESS) {
    LOG_INFO("Failed to init IndexMeta in table:%s, index_name:%s", name(), index_name);
    return rc;
  }
  // 创建索引相关数据
  BplusTreeIndex *index      = new BplusTreeIndex();
  std::string     index_file = table_index_file(base_dir_.c_str(), name(), index_name);
  rc                         = index->create(this, index_file.c_str(), new_index_meta, *field_meta);
  if (rc != RC::SUCCESS) {
    delete index;
    LOG_ERROR("Failed to create bplus tree index. file name=%s, rc=%d:%s", index_file.c_str(), rc, strrc(rc));
    return rc;
  }
  // 遍历当前的所有数据，插入这个索引
  RecordFileScanner scanner;
  rc = get_record_scanner(scanner, trx, ReadWriteMode::READ_ONLY /*readonly*/);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create scanner while creating index. table=%s, index=%s, rc=%s", name(), index_name, strrc(rc));
    return rc;
  }
  Record record;
  while (OB_SUCC(rc = scanner.next(record))) {
    rc = index->insert_entry(record.data(), &record.rid());
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert record into index while creating index. table=%s, index=%s, rc=%s",
          name(),
          index_name,
          strrc(rc));
      return rc;
    }
  }
  if (rc == RC::RECORD_EOF) {
    rc = RC::SUCCESS;
  }

  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to scan records while creating index. table=%s, index=%s, rc=%s", name(), index_name, strrc(rc));
    return rc;
  }
  scanner.close_scan();
  LOG_INFO("inserted all records into new index. table=%s, index=%s", name(), index_name);

  indexes_.push_back(index);

  /// 接下来将这个索引放到表的元数据中
  TableMeta new_table_meta(table_meta_);
  rc = new_table_meta.add_index(new_index_meta);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to add index (%s) on table (%s). error=%d:%s", index_name, name(), rc, strrc(rc));
    return rc;
  }

  /// 内存中有一份元数据，磁盘文件也有一份元数据。修改磁盘文件时，先创建一个临时文件，写入完成后再rename为正式文件
  /// 这样可以防止文件内容不完整
  // 创建元数据临时文件
  std::string  tmp_file = table_meta_file(base_dir_.c_str(), name()) + ".tmp";
  std::fstream fs;
  fs.open(tmp_file, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", tmp_file.c_str(), strerror(errno));
    return RC::IOERR_OPEN; // 创建索引中途出错，要做还原操作
  }
  if (new_table_meta.serialize(fs) < 0) {
    LOG_ERROR("Failed to dump new table meta to file: %s. sys err=%d:%s", tmp_file.c_str(), errno, strerror(errno));
    return RC::IOERR_WRITE;
  }
  fs.close();

  // 覆盖原始元数据文件
  std::string meta_file = table_meta_file(base_dir_.c_str(), name());
  int         ret       = rename(tmp_file.c_str(), meta_file.c_str());
  if (ret != 0) {
    LOG_ERROR("Failed to rename tmp meta file (%s) to normal meta file (%s) while creating index (%s) on table (%s). "
        "system error=%d:%s",
        tmp_file.c_str(),
        meta_file.c_str(),
        index_name,
        name(),
        errno,
        strerror(errno));
    return RC::IOERR_WRITE;
  }

  table_meta_.swap(new_table_meta);

  LOG_INFO("Successfully added a new index (%s) on the table (%s)", index_name, name());
  return rc;
}

RC Table::drop_all_index()
{
  // 遍历并删除所有索引
  for (Index *index : indexes_) {
    const char *index_name = index->index_meta().name();
    string      index_file = table_index_file(base_dir_.c_str(), name(), index_name);

    // 删除索引文件
    if (remove(index_file.c_str()) != 0) {
      LOG_ERROR("Failed to remove index file %s for index %s on table %s, system error=%d:%s",
                index_file.c_str(), index_name, name(), errno, strerror(errno));
      return RC::IOERR_DELETE;
    }

    // 释放内存
    delete index;
  }

  indexes_.clear();

  // 更新表元数据
  TableMeta new_table_meta(table_meta_);
  RC        rc = new_table_meta.remove_all_index();
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to remove all indexes meta for table %s. error=%d:%s", name(), rc, strrc(rc));
    return rc;
  }

  // 更新磁盘上的元数据文件
  string  tmp_file = table_meta_file(base_dir_.c_str(), name()) + ".tmp";
  fstream fs;
  fs.open(tmp_file, ios_base::out | ios_base::binary | ios_base::trunc);
  if (!fs.is_open()) {
    LOG_ERROR("Failed to open file for write. file name=%s, errmsg=%s", tmp_file.c_str(), strerror(errno));
    return RC::IOERR_OPEN;
  }
  if (new_table_meta.serialize(fs) < 0) {
    LOG_ERROR("Failed to dump new table meta to file: %s. sys err=%d:%s", tmp_file.c_str(), errno, strerror(errno));
    return RC::IOERR_WRITE;
  }
  fs.close();

  // 覆盖原始元数据文件
  string meta_file = table_meta_file(base_dir_.c_str(), name());
  int    ret       = rename(tmp_file.c_str(), meta_file.c_str());
  if (ret != 0) {
    LOG_ERROR("Failed to rename tmp meta file (%s) to normal meta file (%s) while dropping all indexes on table (%s). "
              "system error=%d:%s",
              tmp_file.c_str(), meta_file.c_str(), name(), errno, strerror(errno));
    return RC::IOERR_WRITE;
  }

  table_meta_.swap(new_table_meta);
  LOG_INFO("Successfully dropped all indexes on table %s", name());
  return RC::SUCCESS;
}

RC Table::delete_record(const RID &rid)
{
  RC     rc = RC::SUCCESS;
  Record record;
  rc = get_record(rid, record);
  if (OB_FAIL(rc)) {
    return rc;
  }

  return delete_record(record);
}

RC Table::delete_record(const Record &record)
{
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_) {
    rc = index->delete_entry(record.data(), &record.rid());
    if(rc == RC::RECORD_NOT_EXIST) {
      rc = RC::SUCCESS;
    }
  }
  rc = record_handler_->delete_record(&record.rid());
  return rc;
}

RC Table::update_record(const Record &record_)
{
  RC rc = RC::SUCCESS;
  Record origin_record;
  bool error_on_not_exists = true;
  get_record(record_.rid(),origin_record);

  // 存储使用的索引。
  std::vector<Index *> temp_indexes;
  RC rc2 = RC::SUCCESS;
  // 先删除原来的索引。唯一索引和普通索引都会删除。
  for (Index *index : indexes_) {
    rc = index->delete_entry(origin_record.data(), &origin_record.rid());
    if (rc != RC::SUCCESS) {
      LOG_WARN("删除原来的索引失败。");
      break;
    }
    temp_indexes.push_back(index);
  }

  if (rc != RC::SUCCESS) {
    // 删除原有索引失败，重新插入原来的索引。
    for (Index *index : temp_indexes) {
      rc = index->insert_entry(origin_record.data(), &origin_record.rid());
      if (rc != RC::SUCCESS) {
        LOG_WARN("删除原来索引失败，插入原来索引失败。");
        break;
      }
    }
    return rc;
  }
  // 到这里删除原有索引都成功了。尝试插入新索引。如果插入失败，需要重新插入之前的旧索引。
  temp_indexes.clear();
  for (Index *index : indexes_) {
    rc = index->insert_entry(record_.data(), &record_.rid());
    if (rc != RC::SUCCESS) {
      if (rc != RC::RECORD_INVALID_KEY || !error_on_not_exists) {
        LOG_WARN("删除旧索引成功，但是插入新索引失败。");
        break;
      }
    }
    temp_indexes.push_back(index);
  }
  if (rc != RC::SUCCESS) {  // 可能出现了键值重复，导致插入的索引需要被删除。
    // 插入索引失败，删除新插入的索引，还原旧索引。

    for (Index *index : temp_indexes) {
      rc2 = index->delete_entry(record_.data(), &record_.rid());
      if (rc2 != RC::SUCCESS) {
        LOG_WARN("删除新索引失败。。");
        break;
      }
    }
    // 还需要将新元素的索引全部删除
    RC rc3 = insert_entry_of_indexes(origin_record.data(),origin_record.rid());
    if(rc3 != RC::SUCCESS) {
      LOG_WARN("插入失败，删除新索引，回滚旧索引失败。");
    }
    return rc;
  }

  // 插入索引和删除之前的索引都没问题。可以更新值。
  rc    = record_handler_->visit_record(record_.rid(), [&record_](Record &record) {
              record.copy_data(record_.data(), record_.len());
              return true;
  });
  return rc;
}

RC Table::insert_entry_of_indexes(const char *record, const RID &rid)
{
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_) {
    rc = index->insert_entry(record, &rid);
    if (rc != RC::SUCCESS) {
      break;
    }
  }
  return rc;
}

RC Table::delete_entry_of_indexes(const char *record, const RID &rid, bool error_on_not_exists)
{
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_) {
    rc = index->delete_entry(record, &rid);
    if (rc != RC::SUCCESS) {
      if (rc != RC::RECORD_INVALID_KEY || !error_on_not_exists) {
        break;
      }
    }
  }
  return rc;
}

Index *Table::find_index(const char *index_name) const
{
  for (Index *index : indexes_) {
    if (0 == strcmp(index->index_meta().name(), index_name)) {
      return index;
    }
  }
  return nullptr;
}
Index *Table::find_index_by_field(const char *field_name) const
{
  const TableMeta &table_meta = this->table_meta();
  const IndexMeta *index_meta = table_meta.find_index_by_field(field_name);
  if (index_meta != nullptr) {
    return this->find_index(index_meta->name());
  }
  return nullptr;
}
Index* Table::find_index_by_field(const std::vector<string> field_names) const
{
  const TableMeta &table_meta = this->table_meta();
  const IndexMeta *index_meta = table_meta.find_index_by_field(field_names);
  if (index_meta != nullptr) {
    return this->find_index(index_meta->name());
  }
  return nullptr;
}
RC Table::sync()
{
  RC rc = RC::SUCCESS;
  for (Index *index : indexes_) {
    rc = index->sync();
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to flush index's pages. table=%s, index=%s, rc=%d:%s",
          name(),
          index->index_meta().name(),
          rc,
          strrc(rc));
      return rc;
    }
  }

  rc = data_buffer_pool_->flush_all_pages();
  LOG_INFO("Sync table over. table=%s", name());
  return rc;
}

