//
// Created by glwuy on 25-4-13.
//

#ifndef ARITHMETIC_SIMPLIFICATION_RULE_H
#define ARITHMETIC_SIMPLIFICATION_RULE_H


#include "common/sys/rc.h"
#include "sql/optimizer/rewrite_rule.h"

class LogicalOperator;

class ArithmeticSimplificationRule : public ExpressionRewriteRule
{
public:
  ArithmeticSimplificationRule() = default;
  virtual ~ArithmeticSimplificationRule() = default;

  RC rewrite(std::unique_ptr<Expression>& expr, bool& change_made) override;
};


#endif //ARITHMETIC_SIMPLIFICATION_RULE_H
