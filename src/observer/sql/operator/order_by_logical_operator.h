//
// Created by admin on 24-10-23.
//
#pragma once

#include "sql/operator/logical_operator.h"

class OrderByLogicalOperator : public LogicalOperator
{
public:
    OrderByLogicalOperator(
        std::vector<Expression *> &&order_by_exprs,
        std::vector<bool> &&order_by_directions)
        : order_by_expressions_(std::move(order_by_exprs)),
          order_by_directions_(std::move(order_by_directions)) {}

    virtual ~OrderByLogicalOperator() = default;

    LogicalOperatorType type() const override { return LogicalOperatorType::ORDER_BY; }

    // 获取OrderBy的表达式
    auto &order_by_expressions() { return order_by_expressions_; }

    // 获取OrderBy的排序方向
    auto &order_by_directions() { return order_by_directions_; }

private:
    std::vector<Expression *> order_by_expressions_; ///< 用于排序的表达式,其实有继承父类的expression的数组，这个多此一举
    std::vector<bool> order_by_directions_; ///< 每个表达式对应的排序方向，true 为升序，false 为降序
};