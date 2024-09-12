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
  this->expression_ = std::move(expression).release();
}

RC UpdatePhysicalOperator::open(Trx *trx)
{
  auto leftExpr  = std::move(expression_->left());
  auto rightExpr = std::move(expression_->right());

  [[maybe_unused]] std::unique_ptr<FieldExpr> fieldExpression(static_cast<FieldExpr *>(leftExpr.get()));

  [[maybe_unused]] ValueExpr *valueExpression(static_cast<ValueExpr *>(rightExpr.release()));;

  std::unique_ptr<PhysicalOperator> &child = children_[0];

  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  while (OB_SUCC(rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }

    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record &  record    = row_tuple->record();
    records_.emplace_back(std::move(record));
  }

  child->close();

  // 先收集记录再删除
  // 记录的有效性由事务来保证，如果事务不保证删除的有效性，那说明此事务类型不支持并发控制，比如VacuousTrx
  for (Record &record : records_) {
    rc = trx_->delete_record(table_, record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete record: %s", strrc(rc));
      return rc;
    }
  }

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next()
{
  return RC::RECORD_EOF;
}

RC UpdatePhysicalOperator::close() { return RC::SUCCESS; }