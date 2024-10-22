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
    UpdateLogicalOperator(Table* table, std::vector<ComparisonExpr *>& expression)
    {
        this->table_ = table;
        this->set_exprs_.swap(expression);
    }

    Table* table() const { return table_; }
    virtual ~ UpdateLogicalOperator() = default;

    LogicalOperatorType type() const override { return LogicalOperatorType::UPDATE; }

private:
    Table* table_ = nullptr;
    std::vector<ComparisonExpr *> set_exprs_;

public:
    std::vector<ComparisonExpr*>& set_exprs()
    {
        return set_exprs_;
    }

    void set_exprs(const std::vector<ComparisonExpr*>& set_exprs)
    {
        set_exprs_ = set_exprs;
    }
};
