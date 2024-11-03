//
// Created by glwuy on 24-11-1.
//

#ifndef VECTOR_INDEX_SCAN_H
#define VECTOR_INDEX_SCAN_H
#include "sql/expr/tuple.h"
#include "sql/operator/physical_operator.h"

/**
* vctor索引扫描算子。
*/
class VectorIndexScan : public PhysicalOperator
{
public:
    VectorIndexScan(Table* table, Index* index, ReadWriteMode mode, const Value& vector_value);

    virtual ~VectorIndexScan() = default;

    PhysicalOperatorType type() const override { return PhysicalOperatorType::VECTOR_INDEX_SCAN; }

    std::string param() const override;

    RC open(Trx* trx) override;
    RC next() override;
    RC close() override;

    Tuple* current_tuple() override;

private:
    Trx* trx_ = nullptr;
    Table* table_ = nullptr;
    Index* index_ = nullptr;
    Record current_record_;
    RowTuple tuple_;
    ReadWriteMode mode_ = ReadWriteMode::READ_WRITE;
    Value vector_value_;
    IndexScanner* index_scanner_ = nullptr;
    RecordFileHandler* record_handler_ = nullptr;
};


#endif //VECTOR_INDEX_SCAN_H
