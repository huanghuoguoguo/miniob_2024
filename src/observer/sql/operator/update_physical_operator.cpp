//
// Created by bob on 24-9-11.
//

#include "sql/operator/update_physical_operator.h"

#include "project_physical_operator.h"
#include "sql/stmt/update_stmt.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

using namespace std;

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table, std::vector<ComparisonExpr *>& expressions)
{
  this->table_      = table;
  this->expressions_.swap(expressions);
}

RC UpdatePhysicalOperator::open(Trx *trx)
{
  for (auto& e : expressions_) {
    auto expression = e->right().get();
    if (expression != nullptr && expression->type() == ExprType::SUB_QUERY) {
      auto                        sub_query_expr = static_cast<SubQueryExpr*>(expression);
      sub_query_expr->open(trx);
      RC rc = sub_query_expr->check(e->comp());
      if (rc != RC::SUCCESS) return rc;

      // 检查值，设置值的时候只允许一个值，不允许多个值。如果有上面的代码，这里似乎永远不会到达。
      if (!sub_query_expr->is_single_value()) {
        return RC::SUB_QUERY_NUILTI_VALUE;
      }
    }
  }

  std::unique_ptr<PhysicalOperator> &child      = children_[0];

  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_      = trx;

  vector<int> index;
  // 将需要修改的字段的序列保存。
  getColumnIndex(index);

  while (OB_SUCC(rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);


    // 找到所有的value
    int           cell_num = row_tuple->cell_num();
    vector<Value> values;
    for (int i = table_->table_meta().sys_field_num(); i < cell_num; ++i) {
      Value cell;
      row_tuple->cell_at(i, cell);
      values.push_back(cell);
    }
    // 修改对应index的value
    for (size_t i = 0; i < index.size(); i++) {
      int   field_index = index[i];
      Value value;
      expressions_.at(i)->right()->get_value(*row_tuple, value);
      values[field_index] = value;
    }

    Record new_record_test;
    new_record_test.set_rid(row_tuple->record().rid());
    RC rc = table_->make_record(static_cast<int>(values.size()), values.data(), new_record_test);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to make record. rc=%s", strrc(rc));
      return rc;
    }
    records_.push_back(new_record_test);
  }

  child->close();
  for (Record &record : records_) {
    rc = trx_->update_record(table_, record);;
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete record: %s", strrc(rc));
      return rc;
    }
  }

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next() { return RC::RECORD_EOF; }

RC UpdatePhysicalOperator::close()
{
  for (auto& e : expressions_) {
    auto expression = e->right().get();
    if (expression != nullptr && expression->type() == ExprType::SUB_QUERY) {
      auto                        sub_query_expr = static_cast<SubQueryExpr*>(expression);
      RC rc = sub_query_expr->close();
      if (rc != RC::SUCCESS) return rc;
    }
  }
  return RC::SUCCESS;
}

void UpdatePhysicalOperator::getColumnIndex(vector<int> &c)
{
  auto &table_meta  = table_->table_meta();
  auto  field_metas = table_meta.field_metas();
  for (auto &e : expressions_) {
    auto field         = static_cast<FieldExpr *>(e->left().get());
    int  sys_field_num = table_meta.sys_field_num();
    for (int i = sys_field_num; i < static_cast<int>(field_metas->size()); ++i) {
      if (strcmp(field_metas->at(i).name(), field->field_name()) == 0) {
        c.push_back(i - sys_field_num);
        break;
      }
    }
  }
}
