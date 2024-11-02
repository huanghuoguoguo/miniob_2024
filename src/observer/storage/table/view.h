//
// Created by glwuy on 24-10-29.
//

#ifndef VIEW_H
#define VIEW_H
#include <sql/expr/tuple.h>

#include "table.h"
#include <sql/stmt/select_stmt.h>
/**
* 视图可以被当作table直接使用，但是对view的查询，增加，删除，改变，都会影响到原始表。
* 基于单个表的视图：
*   当视图只基于一个表时，通常可以执行增删改查操作（INSERT、DELETE、UPDATE），前提是视图的查询满足某些条件。
*   例如，如果视图中没有聚合函数、GROUP BY、DISTINCT 等复杂操作，那么通常可以对其进行更新。
* 基于多个表的视图：
*   当视图基于多个表时，通常不支持增删改查操作。这是因为 MySQL 无法确定如何将数据更改应用到多个表上。
*   例如，涉及联合（JOIN）、子查询或聚合的视图通常被认为是只读的。
* 例外情况
*   有些情况下，即使是基于单个表的视图，也可能因为特定的查询条件（如涉及计算字段）而无法进行更新。
*/
class View : public Table
{
    // 重写父类的增删改查，并且校验是否能够增删改查。
public:
    View() = default;
    ~View() override;
    RC create_view(Db* db, const char* path, const char* base_dir, int32_t table_id, const char* name,
                   SelectStmt* select_stmt,
                   std::string& sql, std::vector<std::unique_ptr<Expression>>& query_expressions) override;
    RC open(Db* db, const char* meta_file, const char* base_dir) override;
    const char* name() const override{return this->view_name_.c_str();}
    // 真实表会打开一个table，存储表的元数据，view也要进行持久化。
    RC make_record(int value_num, const Value* values, Record& record) override;
    RC insert_record(Record& record) override;
    RC delete_record(const Record& record) override;
    RC delete_record(const RID& rid) override;
    RC update_record(const Record& record) override;
    RC get_record(const RID& rid, Record& record) override;
    RC sync() override;
    RC recover_insert_record(Record& record);

    // 只考虑null_list
    int sys_field_nums() override { return 1; }
    Index* find_index(const char* index_name) const override;
    Index* find_index_by_field(const char* field_name) const override;
    Index* find_index_by_field(const std::vector<string> field_names) const override;
    RC drop_all_index();
    RC create_index(Trx* trx, vector<unique_ptr<Expression>>& column_expressions_, const char* index_name,
                    bool is_unique);
    RC get_record_scanner(RecordFileScanner& scanner, Trx* trx, ReadWriteMode mode);
    RC get_chunk_scanner(ChunkFileScanner& scanner, Trx* trx, ReadWriteMode mode);
    RC write_text(int64_t& offset, int64_t length, const char* data) const;
    RC read_text(int64_t offset, int64_t length, char* data) const;
    RC visit_record(const RID& rid, function<bool(Record &)> visitor);
    RC insert_entry_of_indexes(const char* record, const RID& rid) override;
    RC delete_entry_of_indexes(const char* record, const RID& rid, bool error_on_not_exists) override;


    RC init_record_handler(const char *base_dir) override;
    RC init_text_handler(const char *base_dir) override;
    RC init_vector_handler(const char *base_dir);
    RC set_value_to_record(char *record_data, const Value &value, const FieldMeta *field) override;

    SelectStmt* select_stmt();

    std::vector<TupleCellSpec> & tuple_schemata()
    {
        return tuple_schemata_;
    }



private:
    RC init_tuple_spec();
    RC init_(std::vector<std::unique_ptr<Expression>>& query_expressions);

    string view_name_;
    // 持有真实表的指针。
    std::set<Table*> tables;
    Table* current_table;
    // select_stmt* 原始创建的select。每次打开这个view，是否类似于一个table_scan?打开一个物理计划？
    SelectStmt* selectStmt = nullptr;
    BinderContext* binderContext_ = nullptr;
    std::vector<TupleCellSpec> tuple_schemata_;
    int table_id_ = 0;
    std::vector<std::unique_ptr<Expression>> query_expressions;
    string sql;

};


#endif //VIEW_H
