//
// Created by bob on 24-9-11.
//

#pragma once

#include "sql/operator/physical_operator.h"

class UpdateStmt;

/**
 * @brief 更新物理算子
 * @ingroup PhysicalOperator
 */
class UpdatePhysicalOperator : public PhysicalOperator
{
public:
    UpdatePhysicalOperator() = default;
    UpdatePhysicalOperator(Table* table, std::vector<ComparisonExpr *>& expressions);

    ~UpdatePhysicalOperator() override = default;

    PhysicalOperatorType type() const override { return PhysicalOperatorType::UPDATE; }


    RC open(Trx* trx) override;
    RC next() override;
    RC close() override;



    Tuple* current_tuple() override { return nullptr; }

private:
    Table* table_ = nullptr;
    std::vector<ComparisonExpr *> expressions_;
    Trx* trx_ = nullptr;
    std::vector<Record> records_;

    void getColumnIndex(std::vector<int>& index);
};
