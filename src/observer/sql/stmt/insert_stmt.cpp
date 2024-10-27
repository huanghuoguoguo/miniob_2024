/* Copyright (c) 2021OceanBase and/or its affiliates. All rights reserved.
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

#include "sql/stmt/insert_stmt.h"

#include <common/type/vector_type.h>
#include <sql/parser/expression_binder.h>

#include "common/log/log.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

InsertStmt::InsertStmt(Table *table, const Value *values, int value_amount)
  : table_(table),
    values_(values),
    value_amount_(value_amount)
{
}

RC InsertStmt::create(Db *db, InsertSqlNode &inserts, Stmt *&stmt)
{
  const char *table_name = inserts.relation_name.c_str();
  if (nullptr == db || nullptr == table_name || inserts.values.empty()) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, value_num=%d",
        db,
        table_name,
        static_cast<int>(inserts.values.size()));
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // check the fields number

  // 只考虑了值的情况。
  BinderContext binder_context;
  vector<unique_ptr<Expression>> bound_expressions;
  ExpressionBinder expression_binder(binder_context);

  for (unique_ptr<Expression> &expression : inserts.values) {
    RC rc = expression_binder.bind_expression(expression, bound_expressions);
    if(expression) {
      bound_expressions.emplace_back(std::move(expression));
    }
    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }
  inserts.values.clear();

  std::vector<Value>* values_data = new std::vector<Value>();
  for(auto& bound_expression : bound_expressions) {
    auto value_expr = static_cast<ValueExpr*>(bound_expression.release());
    Value value;
    RC rc = value_expr->try_get_value(value);
    if (OB_FAIL(rc)) {
      LOG_INFO("try get insert value failed. rc=%s", strrc(rc));
      return rc;
    }
    values_data->push_back(value);
  }
  bound_expressions.clear();
  const int        value_num  = static_cast<int>(values_data->size());
  const TableMeta &table_meta = table->table_meta();
  const int        field_num  = table_meta.field_num() - table_meta.sys_field_num();
  const int sys_field_num     = table_meta.sys_field_num();

  if (field_num != value_num) {
    LOG_WARN("schema mismatch. value num=%d, field num in schema=%d", value_num, field_num);
    return RC::SCHEMA_FIELD_MISSING;
  }

  // check field type
  for (int i = 0; i < value_num; i++) {
    const FieldMeta *field_meta = table_meta.field(i + sys_field_num);
    const AttrType   field_type = field_meta->type();
    const AttrType   value_type = values_data->at(i).attr_type();

    // 解决TEXT太长的问题
    if(field_type != value_type) {
      if (AttrType::TEXTS == field_type && AttrType::CHARS == value_type) {
        if(MAX_TEXT_LENGTH < values_data->at(i).length()) {
          LOG_WARN("TEXT_LENGTH:%d IS TOO LONG, longer than 65535",values_data->at(i).length());
          return RC::INVALID_ARGUMENT;
        }
      }
      if (AttrType::VECTORS == field_type) {
        if (value_type == AttrType::VECTORS) {
          // TODO 直接比较。
        } else {
          // char先转vector
          Value v;
          DataType::type_instance(AttrType::CHARS)->cast_to(values_data->at(i), AttrType::VECTORS, v);
          std::vector<float> vector = v.get_vector();
          if (vector.size() != field_meta->len() / sizeof(float)) {
            v.reset();
            return RC::INVALID_ARGUMENT;
          }
        }
      }
    }

  }

  // 不能为null的值为null insert into t1 values(null)
  const std::vector<FieldMeta> *field_metas = table_meta.field_metas();
  for (unsigned long i = 0; i < field_metas->size() - table_meta.sys_field_num(); ++i) {
    const FieldMeta &field_meta = field_metas->at(i + table_meta.sys_field_num());
    Value&           value      = values_data->at(i);
    if (field_meta.nullable() == false && value.is_null()) {
      LOG_WARN("schema mismatch. null field=%d", field_meta.name());
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  }

  // everything alright
  stmt = new InsertStmt(table, values_data->data(), value_num);
  return RC::SUCCESS;
}