//
// Created by glwuy on 24-10-30.
//

#ifndef VIEW_SCAN_PHYSICAL_OPERATOR_H
#define VIEW_SCAN_PHYSICAL_OPERATOR_H

#include "common/rc.h"
#include "sql/operator/physical_operator.h"
#include "storage/record/record_manager.h"
#include "common/types.h"

class View;
class Table;

class ViewScanPhysicalOperator : public PhysicalOperator
{
public:
    ViewScanPhysicalOperator(View* table, ReadWriteMode mode) : view_(table), mode_(mode)
    {
    }

    virtual ~ViewScanPhysicalOperator() = default;

    std::string param() const override;

    PhysicalOperatorType type() const override { return PhysicalOperatorType::VIEW_SCAN; }

    RC open(Trx* trx) override;
    RC next() override;
    RC close() override;

    Tuple* current_tuple() override;


private:
    View* view_ = nullptr;
    Trx* trx_ = nullptr;
    unique_ptr<PhysicalOperator> child_ = nullptr;
    ReadWriteMode                            mode_  = ReadWriteMode::READ_WRITE;
};


#endif //VIEW_SCAN_PHYSICAL_OPERATOR_H
