//
// Created by glwuy on 24-11-1.
//

#include "vector_index_scan_physical_operator.h"
#include "storage/index/index.h"
#include "storage/trx/trx.h"

VectorIndexScan::VectorIndexScan(Table *table, Index *index, ReadWriteMode mode, const Value& vector_value)

{
  this->table_        = table;
  this->index_        = index;
  this->mode_         = mode;
  this->vector_value_ = vector_value;
}

RC VectorIndexScan::open(Trx *trx)
{
  if (nullptr == table_ || nullptr == index_) {
    return RC::INTERNAL;
  }
  IndexScanner *index_scanner = index_->create_scanner(vector_value_.data(),
      vector_value_.length(),
      true,
      vector_value_.data(),
      vector_value_.length(),
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

RC VectorIndexScan::next()
{
  RID rid;
  RC  rc = RC::SUCCESS;


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

RC VectorIndexScan::close()
{
  if(index_scanner_) {
    index_scanner_->destroy();
    index_scanner_ = nullptr;
  }
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