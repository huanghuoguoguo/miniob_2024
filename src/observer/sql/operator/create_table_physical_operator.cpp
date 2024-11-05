//
// Created by lyxahut on 24-11-5.
//

#include "create_table_physical_operator.h"

#include <storage/trx/trx.h>

/**
 * @brief 创建一个新表并插入数据
 * open
 */
RC CreateTablePhysicalOperator::open(Trx *trx)
{
  RC rc=RC::SUCCESS;

  //检查select子查询是否存在
  if(!children_.empty()) {
    auto &child = children_[0];
    rc = child->open(trx);
    if(rc != RC::SUCCESS) {
      LOG_WARN("failed to open child operator, rc=%s",strrc(rc));
      return rc;
    }
  }

  // 创建新表
  rc = db_->create_table(table_name_.c_str(),attr_infos_);
  if(rc != RC::SUCCESS) {
    LOG_WARN("failed to create table %s, rc=%s",table_name_.c_str(), strrc(rc));
    return rc;
  }


  // 如果有孩子节点
  if(! children_.empty()) {
    Table *table = db_->find_table(table_name_.c_str());
    if(table == nullptr) {
      LOG_WARN("failed to find table %s",table_name_.c_str());
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    unique_ptr<PhysicalOperator> &child = children_[0];

    // 如果存在next
    while ((rc = child->next() ) == RC::SUCCESS) {
      Tuple *tuple = child->current_tuple();
      if (tuple == nullptr) {
        LOG_WARN("failed to get current tuple,rc=%s",strrc(rc));
        return rc;
      }

      //创建一个 Record 对象 record，并通过遍历 Tuple 中的每个字段，构建 values 向量，将其填入 record。
      Record record;
      std::vector<Value> values(tuple->cell_num());
      for (int i = 0; i < tuple->cell_num(); i++){
        tuple->cell_at(i, values[i]);
      }

      rc = table->make_record(values.size(),values.data(),record);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to make record. rc=%s", strrc(rc));
        return rc;
      }

      rc = trx->insert_record(table,record);
      if (rc != RC::SUCCESS) {
        //TODO  插入记录失败后删除table
        LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
        return rc;
      }
    }
    if (RC::RECORD_EOF == rc) rc = RC::SUCCESS;
  }
  return rc;
}

RC CreateTablePhysicalOperator::next()
{
  return RC::RECORD_EOF;
}

RC CreateTablePhysicalOperator::close()
{
  RC rc = RC::SUCCESS;
  if (!children_.empty()) {
    rc = children_[0]->close();
    if (RC::SUCCESS != rc) {
      LOG_WARN("failed to close child_operator, rc= %s", strrc(rc));
    }
  }
  return rc;
}

Tuple *CreateTablePhysicalOperator::current_tuple()
{
  return nullptr;
}