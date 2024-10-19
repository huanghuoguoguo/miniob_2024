//
// Created by glwuy on 24-10-19.
//

#pragma once

#include "common/rc.h"
#include "sql/optimizer/rewrite_rule.h"

class LogicalOperator;

class ArithmeticSimplificationRule : public ExpressionRewriteRule
{
public:
    ArithmeticSimplificationRule() = default;
    virtual ~ArithmeticSimplificationRule() = default;

    RC rewrite(std::unique_ptr<Expression>& expr, bool& change_made) override;
};
