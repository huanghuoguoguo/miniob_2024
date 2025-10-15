//
// Created by glwuy on 24-11-1.
//

#include "limit_physical_operator.h"

#include "common/log/log.h"
#include "storage/record/record.h"
#include "storage/table/table.h"



LimitPhysicalOperator::LimitPhysicalOperator(int limit)
{
  this->limit_ = limit;
}

RC LimitPhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  PhysicalOperator *child = children_[0].get();
  RC                rc    = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  return RC::SUCCESS;
}

RC LimitPhysicalOperator::next()
{
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }
  if (limit_ == 0) {
    return RC::RECORD_EOF;
  }
  limit_--;
  return children_[0]->next();
}

RC LimitPhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }
  return RC::SUCCESS;
}
Tuple *LimitPhysicalOperator::current_tuple()
{
  return children_[0]->current_tuple();
}

