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
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "sql/parser/expression_binder.h"

using namespace std;
using namespace common;

SelectStmt::~SelectStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

RC SelectStmt::create(Db *db, SelectSqlNode &select_sql, Stmt *&stmt)
{
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  BinderContext binder_context;

  // collect tables in `from` and `join` statement
  vector<Table *>                tables;
  unordered_map<string, Table *> table_map;
  // 直接将join的表放入需要查询的表中，如果是*则全部获取，如果是table.field也不影响。并且可以参加后续的检查。
  for (size_t i = 0; i < select_sql.join_list.size(); i++) {
    auto& table_name = select_sql.join_list[i].relation;
    select_sql.relations.push_back(table_name);
  }

  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    const char *table_name = select_sql.relations[i].c_str();
    if (nullptr == table_name) {
      LOG_WARN("invalid argument. relation name is null. index=%d", i);
      return RC::INVALID_ARGUMENT;
    }

    Table *table = db->find_table(table_name);
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    binder_context.add_table(table);
    tables.push_back(table);
    table_map.insert({table_name, table});
  }

  // collect query fields in `select` statement
  vector<unique_ptr<Expression>> bound_expressions;
  ExpressionBinder expression_binder(binder_context);

  for (unique_ptr<Expression> &expression : select_sql.expressions) {
    RC rc = expression_binder.bind_expression(expression, bound_expressions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }

  // 将表达式绑定。
  vector<unique_ptr<Expression>> where_expressions;
  for (ConditionSqlNode &expression : select_sql.conditions) {
    std::unique_ptr<Expression> left(expression.left_expr);
    RC                          rc = expression_binder.bind_expression(left, where_expressions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }

    if(!where_expressions.empty()) {
      expression.left_expr = where_expressions[0].release();
    }else {
      expression.left_expr = left.release();
    }
    where_expressions.clear();

    std::unique_ptr<Expression> right(expression.right_expr);
    rc            = expression_binder.bind_expression(right, where_expressions);
    if(!where_expressions.empty()) {
      expression.right_expr = where_expressions[0].release();
    }else {
      expression.right_expr = right.release();
    }

    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }

  // group
  vector<unique_ptr<Expression>> group_by_expressions;
  for (unique_ptr<Expression> &expression : select_sql.group_by) {
    RC rc = expression_binder.bind_expression(expression, group_by_expressions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }

  // create filter statement in `join` statement
  std::vector<pair<Table*,FilterStmt*>> join_filter_stmts;
  for(size_t i = 0; i < select_sql.join_list.size(); i++) {
    FilterStmt *join_filter_stmt = nullptr;
    RC          rc          = FilterStmt::create(db,
        default_table,
        &table_map,
        select_sql.join_list[i].conditions.data(),
        static_cast<int>(select_sql.join_list[i].conditions.size()),
        join_filter_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct join stmt");
      return rc;
    }
    auto& join_table = table_map[select_sql.join_list[i].relation];
    join_filter_stmts.emplace_back(join_table,join_filter_stmt);
  }


  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;
  RC         rc         = FilterStmt::create(db,
      default_table,
      &table_map,
      select_sql.conditions.data(),
      static_cast<int>(select_sql.conditions.size()),
      filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }

  // everything alright
  SelectStmt *select_stmt = new SelectStmt();

  select_stmt->tables_.swap(tables);
  select_stmt->query_expressions_.swap(bound_expressions);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->join_filter_stmts_.swap(join_filter_stmts);
  select_stmt->group_by_.swap(group_by_expressions);
  stmt                      = select_stmt;
  return RC::SUCCESS;
}
