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
  if (select_sql.binder_context == nullptr) {
    select_sql.binder_context = new BinderContext();
    select_sql.binder_context->query_tables().clear();
    select_sql.binder_context->cur_tables().clear();
    select_sql.binder_context->db(db);
  }
  BinderContext& binder_context = *select_sql.binder_context;


  // collect tables in `from` and `join` statement
  vector<Table *>                tables;
  unordered_map<string, Table *> table_map;
  // 直接将join的表放入需要查询的表中，如果是*则全部获取，如果是table.field也不影响。并且可以参加后续的检查。
  for (size_t i = 0; i < select_sql.join_list.size(); i++) {
    for(size_t j = 0; j < select_sql.join_list[i].relations.size(); j++) {
      auto& table_name = select_sql.join_list[i].relations[j];
      select_sql.relations.push_back(table_name);
    }

  }

  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    const char *table_name = select_sql.relations[i].first.c_str();  //拿到的表名是真实表名
    if (nullptr == table_name) {
      LOG_WARN("invalid argument. relation name is null. index=%d", i);
      return RC::INVALID_ARGUMENT;
    }

    Table *table = db->find_table(table_name);
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    binder_context.add_cur_table(table);
    binder_context.add_table(table);
    tables.push_back(table);
    table_map.insert({table_name, table});



    //在这里维护下 表别名和表指针的关系 放到binder_context的as_table_里
    const char *as_name = select_sql.relations[i].second.c_str();
    binder_context.add_as_table(as_name,table_map.find(table_name)->second);

  }


  // collect query fields in `select` statement
  vector<unique_ptr<Expression>> bound_expressions;
  ExpressionBinder expression_binder(binder_context);

  // 绑定搜索列。
  for (unique_ptr<Expression> &expression : select_sql.expressions) {
    RC rc = expression_binder.bind_expression(expression, bound_expressions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }

  // conditions
  RC rc = expression_binder.bind_condition_expression(select_sql.conditions);
  if (OB_FAIL(rc)) {
    LOG_INFO("bind condition expression failed. rc=%s", strrc(rc));
    return rc;
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

  // having和group by只能一起出现，如果没有group by，则不能有having
  if(!select_sql.group_by_having.empty() && group_by_expressions.empty()) {
    return RC::UNSUPPORTED;
  }
  std::vector<FilterStmt*> group_by_having;
  FilterStmt *having_filter_stmt = nullptr;
  // conditions
  rc = expression_binder.bind_condition_expression(select_sql.group_by_having);
  if (OB_FAIL(rc)) {
    LOG_INFO("bind condition expression failed. rc=%s", strrc(rc));
    return rc;
  }

  if(select_sql.group_by_having.size() > 0) {
    rc         = FilterStmt::create(db,
      default_table,
      &table_map,
      select_sql.group_by_having.data(),
      static_cast<int>(select_sql.group_by_having.size()),
      having_filter_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct having filter stmt");
      return rc;
    }
  }



  // create filter statement in `join` statement
  std::vector<pair<Table*,FilterStmt*>> join_filter_stmts;
  for(size_t i = 0; i < select_sql.join_list.size(); i++) {
    FilterStmt *join_filter_stmt = nullptr;
    // conditions
    RC rc = expression_binder.bind_condition_expression(select_sql.join_list[i].conditions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind condition expression failed. rc=%s", strrc(rc));
      return rc;
    }

    rc          = FilterStmt::create(db,
        default_table,
        &table_map,
        select_sql.join_list[i].conditions.data(),
        static_cast<int>(select_sql.join_list[i].conditions.size()),
        join_filter_stmt);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct join stmt");
      return rc;
    }
    auto& join_table = table_map[select_sql.join_list[i].relations[0].first];  //join_list中的relations 直接当作单个pair使用
    join_filter_stmts.emplace_back(join_table,join_filter_stmt);
  }


  // create filter statement in `where` statement
  FilterStmt *filter_stmt = nullptr;
  rc         = FilterStmt::create(db,
      default_table,
      &table_map,
      select_sql.conditions.data(),
      static_cast<int>(select_sql.conditions.size()),
      filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }
  // order
  std::vector<std::unique_ptr<OrderBySqlNode>> order_by_expressions;
  std::vector<std::unique_ptr<Expression>> order_by_bound_expressions; // 存储绑定后的表达式

  // 遍历 `select_sql.order_unit_list` 中的每个 `OrderBySqlNode`
  for ( auto &[expr, is_asc] : select_sql.order_unit_list) {
    // 将原始指针转换为 unique_ptr
    std::unique_ptr<Expression> expr_ptr(expr);

    // 绑定 expression
    RC rc = expression_binder.bind_expression(expr_ptr, order_by_bound_expressions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind order by expression failed. rc=%s", strrc(rc));
      return rc;
    }

    // 创建新的 OrderBySqlNode，并保留 is_asc 属性
    auto order_by_sql_node = std::make_unique<OrderBySqlNode>();
    order_by_sql_node->expr = order_by_bound_expressions.back().release(); // 获取绑定后的表达式并转移所有权
    order_by_sql_node->is_asc = is_asc; // 保留 is_asc 属性

    // 将新的 OrderBySqlNode 添加到 order_by_expressions 中
    order_by_expressions.push_back(std::move(order_by_sql_node));
  }

  // everything alright
  SelectStmt *select_stmt = new SelectStmt();

  select_stmt->tables_.swap(tables);
  select_stmt->query_expressions_.swap(bound_expressions);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->group_by_having_ = having_filter_stmt;
  select_stmt->join_filter_stmts_.swap(join_filter_stmts);
  select_stmt->group_by_.swap(group_by_expressions);
  select_stmt->order_by_.swap(order_by_expressions);
  select_stmt->is_single_ = binder_context.is_single();
  stmt                    = select_stmt;
  return RC::SUCCESS;
}
