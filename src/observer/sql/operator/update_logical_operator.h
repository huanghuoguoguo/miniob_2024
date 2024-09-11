//
// Created by bob on 24-9-10.
//
#pragma once

#include <vector>

#include "sql/operator/logical_operator.h"
/**
 * @brief 插入逻辑算子
 * @ingroup LogicalOperator
 */
class UpdateLogicalOperator : public LogicalOperator
{
public:
  UpdateLogicalOperator(Table *table, std::unique_ptr<Expression> &expression)
  {
    this->table_     = table;
    this->expression = std::move(expression);
  }
  Table   *table() const { return table_; }
  virtual ~                   UpdateLogicalOperator() = default;
  Expression* get_expression() const { return expression.get(); }

  LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }

private:
  Table                      *table_ = nullptr;
  std::unique_ptr<Expression> expression;
  // 属性还有父类的expression 和childrens
};
#include "sql/parser/parse_defs.h"