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
#include "sql/parser/expression_binder.h"

UpdateStmt::~UpdateStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
}

RC UpdateStmt::create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt)
{
  // TODO
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  BinderContext binder_context;

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
  // 验证字段名

  const std::string &field_name = update_sql.attribute_name; // 属性名
  const Value &      value      = update_sql.value;

  //创建字段表达式和值表达式
  unique_ptr<Expression> unbound_field_expr = std::make_unique<UnboundFieldExpr>(table_name, field_name);
  unique_ptr<Expression> value_expr         = std::make_unique<ValueExpr>(value);

  // 2. 创建一个未绑定的比较表达式，用于表示 field_name = value
  std::unique_ptr<Expression> unbound_comp_expr = std::make_unique<ComparisonExpr>(
      EQUAL_TO,
      std::move(unbound_field_expr),
      // 使用std::move转移所有权
      std::move(value_expr) // 使用std::move转移所有权
      );

  // 创建表达式绑定器并执行绑定操作
  vector<unique_ptr<Expression>> bound_expressions;
  ExpressionBinder               expression_binder(binder_context);

  RC rc = expression_binder.bind_expression(unbound_comp_expr, bound_expressions);
  if (OB_FAIL(rc)) {
    LOG_INFO("bind expression failed. rc=%s", strrc(rc));
    return rc;
  }

  if (rc == RC::SUCCESS) {
    // 如果绑定成功，bound_expressions 中将包含绑定后的比较表达式
    LOG_INFO("Successfully bound the comparison expression: %s = %s", field_name.c_str(), value.to_string().c_str());
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
  update_stmt->attribute_name_ = update_sql.attribute_name; //不需要了，可以获得
  update_stmt->value_          = update_sql.value;          //不需要了，可以获得
  update_stmt->filter_stmt_    = filter_stmt;
  ComparisonExpr *expression   = static_cast<ComparisonExpr *>(bound_expressions[0].release());

  update_stmt->comparisonExpr = std::unique_ptr<ComparisonExpr>(expression);
  stmt                        = update_stmt;
  return RC::SUCCESS;
}