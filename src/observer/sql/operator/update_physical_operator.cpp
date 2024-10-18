//
// Created by bob on 24-9-11.
//

#include "sql/operator/update_physical_operator.h"
#include "sql/stmt/update_stmt.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

using namespace std;

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table, unique_ptr<ComparisonExpr> expression)
{
  this->table_      = table;
  this->expression_ = std::move(expression);
}

RC UpdatePhysicalOperator::open(Trx *trx)
{
  auto leftExpr  = expression_->left().get();
  auto rightExpr = expression_->right().get();

  FieldExpr *fieldExpression(static_cast<FieldExpr *>(leftExpr));

  ValueExpr *valueExpression(static_cast<ValueExpr *>(rightExpr));

  auto                               field_name = fieldExpression->field().field_name();
  auto                               table      = fieldExpression->field().table();
  auto                               value      = valueExpression->get_value();
  std::unique_ptr<PhysicalOperator> &child      = children_[0];

  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_      = trx;
  int index = -1;
  while (OB_SUCC(rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);

    if (index == -1) {
      // 找到index
      TupleCellSpec tupleCellSpec;
      int           cell_num = row_tuple->cell_num();
      for (int i = table->table_meta().sys_field_num(); i < cell_num; i++) {
        row_tuple->spec_at(i, tupleCellSpec);
        if (strcmp(tupleCellSpec.field_name(), field_name) == 0) {
          index = i - table->table_meta().sys_field_num();
          break;
        }
      }
    }
    // 找到所有的value
    int           cell_num = row_tuple->cell_num();
    vector<Value> values;
    for (int i = table->table_meta().sys_field_num(); i < cell_num; ++i) {
      Value cell;
      row_tuple->cell_at(i, cell);
      values.push_back(cell);
    }
    // 修改对应index的value
    values[index] = value;

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
  // return children()[0]->close();
  return RC::SUCCESS;
}