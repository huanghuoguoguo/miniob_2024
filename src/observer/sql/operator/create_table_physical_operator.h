//
// Created by lyxahut on 24-11-5.
//

#ifndef CREATE_TABLE_PHYSICAL_OPERATOR_H
#define CREATE_TABLE_PHYSICAL_OPERATOR_H
#include "physical_operator.h"

#include <storage/db/db.h>

#include <utility>


/**
 * @brief 建表物理操作符
 */
class CreateTablePhysicalOperator : public PhysicalOperator
{
public:
  CreateTablePhysicalOperator(Db *db, std::string table_name, std::vector<AttrInfoSqlNode> attr_infos)
      : db_(db), table_name_(std::move(table_name)), attr_infos_(std::move(attr_infos))
  {}

  virtual ~CreateTablePhysicalOperator() = default;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::CREATE_TABLE;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;
  Tuple * current_tuple() override;


private:
  Db *db_=nullptr;
  std::string table_name_;
  std::vector<AttrInfoSqlNode> attr_infos_;
};

#endif //CREATE_TABLE_PHYSICAL_OPERATOR_H
