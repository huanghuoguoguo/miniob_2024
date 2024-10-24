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
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "sql/parser/expression_binder.h"
#include "storage/db/db.h"

UpdateStmt::~UpdateStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

RC UpdateStmt::create(Db *db, UpdateSqlNode &update_sql, Stmt *&stmt)
{
  RC rc = RC::SUCCESS;
  // TODO
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  BinderContext binder_context;
  binder_context.db(db);

  // 找到需要更新的表
  const char *table_name = update_sql.relation_name.c_str();
  if (nullptr == table_name) {
    LOG_WARN("invalid argument. table name is null");
    return RC::INVALID_ARGUMENT;
  }

  // 把表存到table对象中
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  binder_context.add_table(table);




  // 创建表达式绑定器并执行绑定操作
  vector<unique_ptr<Expression>> bound_expressions;
  ExpressionBinder               expression_binder(binder_context);
  rc = expression_binder.bind_condition_expression(update_sql.set_expression);
  if(OB_FAIL(rc)) {
    return rc;
  }

  std::vector<ComparisonExpr*> set_exprs;
  // 检查，不能为null的值不允许添加为null，操作符必须为=
  for (auto &node : update_sql.set_expression) {
    if (node.comp != EQUAL_TO) {
      return RC::INVALID_ARGUMENT;
    }
    if (node.left_expr->type() != ExprType::FIELD) {
      return RC::INVALID_ARGUMENT;
    }
    if (node.right_expr->type() == ExprType::VALUE) {
      // 如果是值，可以继续判断，如果是子查询，延后判断。
      FieldExpr *field_expr = static_cast<FieldExpr *>(node.left_expr);
      ValueExpr *value_expr = static_cast<ValueExpr *>(node.right_expr);
      if (!field_expr->field().meta()->nullable() && value_expr->value_type() == AttrType::UNDEFINED) {
        return RC::INVALID_ARGUMENT;
      }
    }else {
      // 如果是多列值，返回错误。
      SubQueryExpr *sub_query_expr = static_cast<SubQueryExpr *>(node.right_expr);
      if (sub_query_expr->select_stmt() != nullptr && sub_query_expr->select_stmt()->query_expressions().size() > 1) {
        return RC::SUB_QUERY_NUILTI_COLUMN;
      }
    }

    // 构造ComparisonExpr
    ComparisonExpr * set_e = new ComparisonExpr(EQUAL_TO,std::unique_ptr<Expression>(node.left_expr),std::unique_ptr<Expression>(node.right_expr));
    set_exprs.push_back(set_e);
  }




  // conditions
  rc = expression_binder.bind_condition_expression(update_sql.conditions);
  if (OB_FAIL(rc)) {
    LOG_INFO("bind condition expression failed. rc=%s", strrc(rc));
    return rc;
  }
  // 处理WHERE子句
  FilterStmt *filter_stmt = nullptr;
  rc                      = FilterStmt::create(db,
      table,
      nullptr,
      // 由于只涉及单个表，传递空的table_map
      update_sql.conditions.data(),
      static_cast<int>(update_sql.conditions.size()),
      filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    return rc;
  }

  // everything alright
  UpdateStmt *update_stmt = new UpdateStmt();

  update_stmt->table_          = table;
  update_stmt->filter_stmt_    = filter_stmt;
  update_stmt->set_values_.swap(set_exprs);
  stmt                        = update_stmt;
  return RC::SUCCESS;
}