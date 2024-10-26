/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2022/6/27.
//

#include "sql/operator/predicate_physical_operator.h"

#include <sql/expr/composite_tuple.h>

#include "project_physical_operator.h"
#include "common/log/log.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/field/field.h"
#include "storage/record/record.h"

PredicatePhysicalOperator::PredicatePhysicalOperator(std::unique_ptr<Expression> expr) : expression_(std::move(expr))
{
  ASSERT(expression_->value_type() == AttrType::BOOLEANS, "predicate's expression should be BOOLEAN type");
}

RC PredicatePhysicalOperator::open(Trx *trx)
{
  if (children_.size() != 1) {
    LOG_WARN("predicate operator must has one child");
    return RC::INTERNAL;
  }
  std::vector<Expression*> children_expr;

  if (expression_->type() == ExprType::COMPARISON) {
    children_expr.push_back(expression_.get());
  }
  else if(expression_->type() == ExprType::CONJUNCTION) {
    auto                                             conjunction_expr = static_cast<ConjunctionExpr *>(expression_.get());
    std::vector<std::unique_ptr<Expression>>& children         = conjunction_expr->children();
    for(auto& child : children) {
      children_expr.push_back(child.get());
    }
  }

  for (auto &child : children_expr) {
    if (child->type() == ExprType::COMPARISON) {
      auto comparison_expr = static_cast<ComparisonExpr *>(child);
      auto left            = comparison_expr->left().get();
      auto right           = comparison_expr->right().get();
      if (left->type() == ExprType::SUB_QUERY) {
        SubQueryExpr *left_sub_query_expr = static_cast<SubQueryExpr *>(left);
        RC            rc                  = left_sub_query_expr->open(trx);
        if (rc != RC::SUCCESS) { return rc; }
        rc                                = left_sub_query_expr->check(comparison_expr->comp());
        if (rc != RC::SUCCESS)
          return rc;
      }
      if (right->type() == ExprType::SUB_QUERY) {
        SubQueryExpr *right_sub_query_expr = static_cast<SubQueryExpr *>(right);
        RC            rc                   = right_sub_query_expr->open(trx);
        if (rc != RC::SUCCESS) { return rc; }
        rc = right_sub_query_expr->check(comparison_expr->comp());
        if (rc != RC::SUCCESS)
          return rc;
      }
    }
  }
  return children_[0]->open(trx);
}

RC PredicatePhysicalOperator::next()
{
  RC                rc   = RC::SUCCESS;
  PhysicalOperator *oper = children_.front().get();

  while (RC::SUCCESS == (rc = oper->next())) {
    Tuple *tuple = oper->current_tuple();
    if (nullptr == tuple) {
      rc = RC::INTERNAL;
      LOG_WARN("failed to get tuple from operator");
      break;
    }
    // 如果父传过来的tuple不为空，拼接一下。
    if (!values_.empty()) {
      auto *target_tuple      = new CompositeTuple();
      auto *origin_tuple_copy = new ValueListTuple();
      rc                      = ValueListTuple::make(*tuple, *origin_tuple_copy);
      if (OB_FAIL(rc)) {
        return rc;
      }

      target_tuple->add_tuple(std::unique_ptr<Tuple>(origin_tuple_copy));

      for (auto &const_value_tuple : values_) {
        auto *temp = new ValueListTuple();
        rc         = ValueListTuple::make(const_value_tuple, *temp);
        if (OB_FAIL(rc)) {
          return rc;
        }
        target_tuple->add_tuple(std::unique_ptr<Tuple>(temp));
      }
      tuple = target_tuple;
    }


    Value value;
    rc = expression_->get_value(*tuple, value);
    if(rc == RC::DIVIDE_ZERO) {
      continue;
    }
    if (rc != RC::SUCCESS) {
      return rc;
    }

    if (value.get_boolean()) {
      return rc;
    }
  }
  return rc;
}

RC PredicatePhysicalOperator::close()
{
  children_[0]->close();
  if (expression_->type() == ExprType::COMPARISON) {
    auto comparison_expr = static_cast<ComparisonExpr *>(expression_.get());
    auto left            = comparison_expr->left().get();
    auto right           = comparison_expr->right().get();
    if (left->type() == ExprType::SUB_QUERY) {
      SubQueryExpr *left_sub_query_expr = static_cast<SubQueryExpr *>(left);
      if (left_sub_query_expr->phy_op() != nullptr) {
        left_sub_query_expr->phy_op()->close();
      }
    }
    if (right->type() == ExprType::SUB_QUERY) {
      SubQueryExpr *right_sub_query_expr = static_cast<SubQueryExpr *>(right);
      if (right_sub_query_expr->phy_op() != nullptr) {
        right_sub_query_expr->phy_op()->close();
      }
    }
  }
  return RC::SUCCESS;
}

Tuple *PredicatePhysicalOperator::current_tuple() { return children_[0]->current_tuple(); }

RC PredicatePhysicalOperator::tuple_schema(TupleSchema &schema) const
{
  return children_[0]->tuple_schema(schema);
}
