//
// Created by glwuy on 24-10-30.
//

#include "view_scan_physical_operator.h"

#include <sql/optimizer/logical_plan_generator.h>
#include <sql/optimizer/physical_plan_generator.h>
#include <sql/optimizer/rewriter.h>
#include <storage/table/view.h>

#include "project_physical_operator.h"

std::string ViewScanPhysicalOperator::param() const
{
  return view_->name();
}


RC ViewScanPhysicalOperator::open(Trx *trx)
{
  this->trx_ = trx;
  RC rc = RC::SUCCESS;
  if (child_ == nullptr) {
    // 从view中获取到stmt，然后从stmt到逻辑计划，再从逻辑计划得到物理计划。
    LogicalPlanGenerator  logical_plan_generator_;   ///< 根据SQL生成逻辑计划
    PhysicalPlanGenerator physical_plan_generator_;  ///< 根据逻辑计划生成物理计划
    Rewriter              rewriter_;                 ///< 逻辑计划改写
    unique_ptr<LogicalOperator> logical_operator;
    rc = logical_plan_generator_.create(view_->select_stmt(),logical_operator);
    if(rc!=RC::SUCCESS) {
      return rc;
    }
    bool change_made = false;
    do {
      change_made = false;
      rc          = rewriter_.rewrite(logical_operator, change_made);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to do expression rewrite on logical plan. rc=%s", strrc(rc));
        return rc;
      }
    } while (change_made);
    unique_ptr<PhysicalOperator> physical_operator;
    physical_plan_generator_.create(*logical_operator,physical_operator);
    this->child_ = std::move(physical_operator);
  }
  return child_->open(trx);
}

RC ViewScanPhysicalOperator::next()
{
  return child_->next();
}

RC ViewScanPhysicalOperator::close()
{

  return child_->close();
}

Tuple *ViewScanPhysicalOperator::current_tuple()
{
  Tuple *child_tuple = child_->current_tuple();
  ValueListTuple* tuple = new ValueListTuple();
  ValueListTuple::make(*child_tuple,view_->tuple_schemata(),*tuple);
  return tuple;
}