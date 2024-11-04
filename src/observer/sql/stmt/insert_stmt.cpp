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

  const TableMeta &table_meta = table->table_meta();
  const int        field_num  = table_meta.field_num() - table_meta.sys_field_num();
  const int sys_field_num     = table_meta.sys_field_num();

  // check the fields number

  // 只考虑了值的情况。
  BinderContext binder_context;
  binder_context.add_table(table);
  vector<unique_ptr<Expression>> bound_values_expressions;
  vector<unique_ptr<Expression>> bound_field_expressions;
  ExpressionBinder expression_binder(binder_context);

  // 开始绑定字段和值
  for (unique_ptr<Expression> &expression : inserts.columns) {
    if (RC rc = expression_binder.bind_expression(expression, bound_field_expressions); OB_FAIL(rc)) {
      LOG_INFO("bind field expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }
  for (unique_ptr<Expression> &expression : inserts.values) {
    RC rc = expression_binder.bind_expression(expression, bound_values_expressions);
    if(expression) {
      bound_values_expressions.emplace_back(std::move(expression));
    }
    if (OB_FAIL(rc)) {
      LOG_INFO("bind value expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }
  inserts.values.clear();
  inserts.columns.clear();

  // 如果绑定列不为空，可能需要将值的位置重新排列。其他位置填充null
  std::vector<Value> *values_data = new std::vector<Value>(field_num,Value());
  if (bound_field_expressions.empty()) {
    int i = 0;
    for (auto &bound_expression : bound_values_expressions) {
      Value value;
      RC    rc = bound_expression->try_get_value(value);
      if (OB_FAIL(rc)) {
        LOG_INFO("try get insert value failed. rc=%s", strrc(rc));
        delete values_data;
        return rc;
      }
      (*values_data)[i] = value;
      i++;
    }
  } else {
    if (bound_field_expressions.size() != bound_values_expressions.size()) {
      delete values_data;
      return RC::INVALID_ARGUMENT;
    }
    // 将其重排并且填充。
    // 根据绑定的每个field，找到其在values_data中的位置。
    int i = 0;
    for(auto& field_expression : bound_field_expressions) {
      FieldExpr * field_expr = dynamic_cast<FieldExpr*>(field_expression.get());
      if(field_expr) {
        Field&           field      = field_expr->field();
        const FieldMeta *field_meta = field.meta();
        int              field_id   = field_meta->field_id();
        // 将值放到对应的位置上。
        auto& bound_expression = bound_values_expressions.at(i);
        Value value;
        RC    rc = bound_expression->try_get_value(value);
        if (OB_FAIL(rc)) {
          LOG_INFO("try get insert value failed. rc=%s", strrc(rc));
          delete values_data;
          return rc;
        }
        (*values_data)[field_id - sys_field_num] = value;
        i++;
      } else {
        delete values_data;
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }
    }

  }







  const int        value_num  = static_cast<int>(values_data->size());


  if (field_num != value_num) {
    LOG_WARN("schema mismatch. value num=%d, field num in schema=%d", value_num, field_num);
    delete values_data;
    return RC::SCHEMA_FIELD_MISSING;
  }

  // check field type
  for (int i = 0; i < value_num; i++) {
    const FieldMeta *field_meta = table_meta.field(i + sys_field_num);
    Value&             val        = values_data->at(i);
    const AttrType   field_type = field_meta->type();
    const AttrType   value_type = val.attr_type();

    // 解决TEXT太长的问题
    if (field_type != value_type) {
      if (AttrType::TEXTS == field_type && AttrType::CHARS == value_type) {
        if (MAX_TEXT_LENGTH < val.length()) {
          LOG_WARN("TEXT_LENGTH:%d IS TOO LONG, longer than 65535", values_data->at(i).length());
          delete values_data;
          return RC::INVALID_ARGUMENT;
        }
      }
    }
    if (AttrType::VECTORS == field_type) {
      if (val.attr_type() == AttrType::CHARS) {
        // char先转vector
        Value v;
        DataType::type_instance(AttrType::CHARS)->cast_to(val, AttrType::VECTORS, v);
        // 直接将其替换为vector类型。
        val = v;
      } else if (val.attr_type() != AttrType::VECTORS) {
        delete values_data;
        return RC::INVALID_ARGUMENT;
      }
      // 此时已经是vector类型，判断维度是否一致。
      // std::vector<float> vector = val.get_vector();
      // if (vector.size() != field_meta->is_high_dim()) {
      //   delete values_data;
      //   return RC::INVALID_ARGUMENT;
      // }
      // 少一次拷贝。
      if (val.length() / sizeof(float) != field_meta->is_high_dim()) {
        delete values_data;
        return RC::INVALID_ARGUMENT;
      }
    }
    // 检查结束。
  }

  // 不能为null的值为null insert into t1 values(null)
  const std::vector<FieldMeta> *field_metas = table_meta.field_metas();
  for (unsigned long i = 0; i < field_metas->size() - table_meta.sys_field_num(); ++i) {
    const FieldMeta &field_meta = field_metas->at(i + table_meta.sys_field_num());
    Value&           value      = values_data->at(i);
    if (field_meta.nullable() == false && value.is_null()) {
      LOG_WARN("schema mismatch. null field=%d", field_meta.name());
      delete values_data;
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  }

  // everything alright

  stmt = new InsertStmt(table, values_data->data(), value_num);
  return RC::SUCCESS;
}