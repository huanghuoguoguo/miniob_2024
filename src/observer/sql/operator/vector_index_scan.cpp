//
// Created by glwuy on 24-11-1.
//

#include "vector_index_scan.h"
#include "storage/index/index.h"
#include "storage/trx/trx.h"

VectorIndexScan::VectorIndexScan(Table *table, Index *index, ReadWriteMode mode, const Value vector_value)

{
  this->table_        = table;
  this->index_        = index;
  this->mode_         = mode;
  this->vector_value_ = vector_value;
}

RC VectorIndexScan::open(Trx *trx)
{
  return RC::SUCCESS;
}

RC VectorIndexScan::next()
{
  RC rc = RC::SUCCESS;
  return rc;
}

RC VectorIndexScan::close()
{

  return RC::SUCCESS;
}

Tuple *VectorIndexScan::current_tuple()
{
  tuple_.set_record(&current_record_);
  return &tuple_;
}


std::string VectorIndexScan::param() const
{
  return std::string(index_->index_meta().name()) + " ON " + table_->name();
}