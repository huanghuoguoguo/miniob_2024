//
// Created by glwuy on 24-11-1.
//

#ifndef LIMIT_LOGICAL_OPERATOR_H
#define LIMIT_LOGICAL_OPERATOR_H

#include "sql/operator/physical_operator.h"

class LimitPhysicalOperator : public PhysicalOperator
{
public:
    LimitPhysicalOperator(int limit);
    virtual ~LimitPhysicalOperator() = default;

    PhysicalOperatorType type() const override { return PhysicalOperatorType::LIMIT; }

    RC open(Trx* trx) override;
    RC next() override;
    RC close() override;
    Tuple* current_tuple() override;

private:
    int limit_ = -1;
};


#endif //LIMIT_LOGICAL_OPERATOR_H
