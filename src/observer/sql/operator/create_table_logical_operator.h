//
// Created by lyxahut on 24-11-5.
//
#include "logical_operator.h"

class Db;
/**
 * @brief CreateTable Logical Operator
 */
class CreateTableLogicalOperator : public LogicalOperator
{
public:
  CreateTableLogicalOperator(Db *db, const std::string &table_name, const std::vector<AttrInfoSqlNode> &attr_infos)
      : db_(db), table_name_(table_name), attr_infos_(attr_infos)
  {}

  virtual ~CreateTableLogicalOperator() = default;

  //Db
  Db *get_db() { return db_; }

  // 类型
  LogicalOperatorType type() const override { return LogicalOperatorType::CREATE_TABLE; }

  // table_name
  std::string &table_name() { return table_name_; }

  // attr_infos
  const std::vector<AttrInfoSqlNode> &attr_infos() const { return attr_infos_; }


private:
  Db *db_=nullptr;
  std::string table_name_;
  std::vector<AttrInfoSqlNode> attr_infos_;
};




