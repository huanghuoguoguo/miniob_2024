//
// Created by glwuy on 24-10-26.
//

#include "operator_iterator.h"




RC OperatorIterator::iterate_child_oper(PhysicalOperator *expr,
    std::function<RC(PhysicalOperator *)>                 callback)
{
  if (expr == nullptr) {
    return RC::SUCCESS;
  }

  RC rc = callback(expr);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  for (auto &e : expr->children()) {
    rc = iterate_child_oper(e.get(), callback);
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }
  return RC::SUCCESS;
}