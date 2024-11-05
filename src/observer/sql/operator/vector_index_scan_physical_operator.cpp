//
// Created by glwuy on 24-11-1.
//

#include "vector_index_scan_physical_operator.h"
#include "storage/index/index.h"
#include "storage/trx/trx.h"

VectorIndexScanPhysicalOperator::VectorIndexScanPhysicalOperator(Table *table, Index *index, ReadWriteMode mode, const Value& vector_value)

{
  this->table_        = table;
  this->index_        = index;
  this->mode_         = mode;
  this->vector_value_ = vector_value;
}

RC VectorIndexScanPhysicalOperator::open(Trx *trx)
{
  if (nullptr == table_ || nullptr == index_) {
    return RC::INTERNAL;
  }
  Value limit(this->limit_);
  limit.set_type(AttrType::INTS);
  IndexScanner *index_scanner = index_->create_scanner(vector_value_.data(),
      vector_value_.length(),
      true,
      limit.data(),
      limit.length(),
      true);
  if (nullptr == index_scanner) {
    LOG_WARN("failed to create index scanner");
    return RC::INTERNAL;
  }

  record_handler_ = table_->record_handler();
  if (nullptr == record_handler_) {
    LOG_WARN("invalid record handler");
    index_scanner->destroy();
    return RC::INTERNAL;
  }
  index_scanner_ = index_scanner;

  tuple_.set_schema(table_, table_->table_meta().field_metas());

  trx_ = trx;
  return RC::SUCCESS;
}

RC VectorIndexScanPhysicalOperator::next()
{
  RID rid;
  RC  rc = RC::SUCCESS;
  if (this->limit_ <= 0) {
    return RC::RECORD_EOF;
  }
  this->limit_--;

  while (RC::SUCCESS == (rc = index_scanner_->next_entry(&rid))) {
    rc = record_handler_->get_record(rid, current_record_);
    if (OB_FAIL(rc)) {
      LOG_TRACE("failed to get record. rid=%s, rc=%s", rid.to_string().c_str(), strrc(rc));
      return rc;
    }

    LOG_TRACE("got a record. rid=%s", rid.to_string().c_str());

    tuple_.set_record(&current_record_);
    if (OB_FAIL(rc)) {
      LOG_TRACE("failed to filter record. rc=%s", strrc(rc));
      return rc;
    }

    rc = trx_->visit_record(table_, current_record_, mode_);
    if (rc == RC::RECORD_INVISIBLE) {
      LOG_TRACE("record invisible");
      continue;
    } else {
      return rc;
    }
  }

  return rc;
}

RC VectorIndexScanPhysicalOperator::close()
{
  if(index_scanner_) {
    index_scanner_->destroy();
    index_scanner_ = nullptr;
  }
  return RC::SUCCESS;
}

Tuple *VectorIndexScanPhysicalOperator::current_tuple()
{
  tuple_.set_record(&current_record_);
  return &tuple_;
}


std::string VectorIndexScanPhysicalOperator::param() const
{
  return std::string(index_->index_meta().name()) + " ON " + table_->name();
}