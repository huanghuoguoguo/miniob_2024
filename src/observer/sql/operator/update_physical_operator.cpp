//
// Created by bob on 24-9-11.
//

#include "sql/operator/update_physical_operator.h"

#include <sql/expr/composite_tuple.h>

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

  bool is_open = false;
  while (OB_SUCC(rc = child->next())) {
    // 如果返回多个tuple。返回错误。
    if(!is_open) {
      // 延迟到匹配到任何行时才进行检查。
      for (auto& e : expressions_) {
        auto expression = e->right().get();
        if (expression != nullptr && expression->type() == ExprType::SUB_QUERY) {
          auto                        sub_query_expr = static_cast<SubQueryExpr*>(expression);
          rc = sub_query_expr->open(trx);
          if(rc != RC::SUCCESS) {
            return rc;
          }
          // 如果返回多个tuple。返回错误。
          if (!sub_query_expr->is_single_tuple()) {
            return RC::SUB_QUERY_NUILTI_VALUE;
          }
        }
      }
      is_open = true;
    }
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = dynamic_cast<RowTuple *>(tuple);
    if (row_tuple == nullptr) {
      CompositeTuple *composite_tuple = dynamic_cast<CompositeTuple *>(tuple);
      if (composite_tuple) {
        std::vector<std::unique_ptr<Tuple>>& tuples = composite_tuple->tuples();
        for (auto &tuple : tuples) {
          Tuple *sub_tuple = dynamic_cast<Tuple *>(tuple.get());
          if (dynamic_cast<RowTuple *>(sub_tuple)) {
            row_tuple = dynamic_cast<RowTuple *>(sub_tuple);
          }
        }
      }
    }


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
      LOG_WARN("failed to update record: %s", strrc(rc));
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
