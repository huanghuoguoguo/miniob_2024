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

RC SelectStmt::check_tabel(Db *db, BinderContext& binder_context, vector<Table *>& tables,
    unordered_map<string, Table *>& table_map, size_t i, const char *table_name)
{
  if (nullptr == table_name) {
    LOG_WARN("invalid argument. relation name is null. index=%d", i);
    return  RC::INVALID_ARGUMENT;
  }

  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return  RC::SCHEMA_TABLE_NOT_EXIST;
  }
  if (!table_map.contains(table_name)) {
    binder_context.add_table(table);
    tables.push_back(table);
    table_map.insert({table_name, table});
  }
  return RC::SUCCESS;
}
RC SelectStmt::create(Db *db, SelectSqlNode &select_sql, Stmt *&stmt)
{
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  BinderContext binder_context;

  // collect tables in `from` statement
  vector<Table *>                tables;
  unordered_map<string, Table *> table_map;

  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    const char *table_name = select_sql.relations[i].relation.c_str();
    RC          rc         = RC::SUCCESS;
    if ((rc = check_tabel(db, binder_context, tables, table_map, i, table_name)) != RC::SUCCESS) {
      return rc;
    }

    const char *join_table_name = select_sql.relations[i].join_relation.c_str();
    if ((rc = check_tabel(db, binder_context, tables, table_map, i, join_table_name)) != RC::SUCCESS) {
      return rc;
    }

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
  std::vector<std::tuple<Table *, Table *, FilterStmt *>> join_filter_stmts;
  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    if (select_sql.relations[i].op != JOIN_NO_OP) {
      FilterStmt *join_filter_stmt = nullptr;
      RC          rc               = FilterStmt::create(db,
          default_table,
          &table_map,
          select_sql.relations[i].conditions.data(),
          static_cast<int>(select_sql.relations[i].conditions.size()),
          join_filter_stmt);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot construct join stmt");
        return rc;
      }
      auto &left_join_table  = table_map[select_sql.relations[i].relation];
      auto &right_join_table = table_map[select_sql.relations[i].join_relation];
      join_filter_stmts.emplace_back(left_join_table, right_join_table, join_filter_stmt);
    }
  }


  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;
  RC          rc          = FilterStmt::create(db,
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
  select_stmt->join_filter_stmts_.swap(join_filter_stmts);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->group_by_.swap(group_by_expressions);
  stmt                      = select_stmt;
  return RC::SUCCESS;
}
