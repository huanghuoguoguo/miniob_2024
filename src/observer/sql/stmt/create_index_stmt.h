/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/4/25.
//

#pragma once

#include <string>

#include "sql/stmt/stmt.h"

struct CreateIndexSqlNode;
class Table;
class FieldMeta;

/**
 * @brief 创建索引的语句
 * @ingroup Statement
 */
class CreateIndexStmt : public Stmt
{
public:
  CreateIndexStmt()
  {}

  virtual ~CreateIndexStmt() = default;

  StmtType type() const override { return StmtType::CREATE_INDEX; }

  Table             *table() const { return table_; }
  const std::string &index_name() const { return index_name_; }

  vector<unique_ptr<Expression>>& column_expressions()
  {
    return column_expressions_;
  }

  string index_type() const
  {
    return index_type_;
  }

  void index_type(const string& index_type)
  {
    this->index_type_ = index_type;
  }

  vector<ConditionSqlNode>& with_expressions()
  {
    return with_expressions_;
  }

  void with_expressions(const vector<ConditionSqlNode>& with_expressions)
  {
    with_expressions_ = with_expressions;
  }

  bool unique() const
  {
    return unique_;
  }

  static RC create(Db *db, CreateIndexSqlNode &create_index, Stmt *&stmt);

private:
  Table           *table_      = nullptr;
  vector<unique_ptr<Expression>> column_expressions_;
  std::string      index_name_;
  bool unique_ = false;
  string index_type_ = "normal";
  vector<ConditionSqlNode> with_expressions_;
};
