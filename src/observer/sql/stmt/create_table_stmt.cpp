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
// Created by Wangyunlai on 2023/6/13.
//

#include "common/log/log.h"
#include "common/types.h"
#include "sql/stmt/create_table_stmt.h"

#include "select_stmt.h"
#include "event/sql_debug.h"

#include <sql/expr/expression.h>

RC CreateTableStmt::create(Db *db, const CreateTableSqlNode &create_table, Stmt *&stmt, SelectSqlNode &select_sql)
{
  StorageFormat storage_format = StorageFormat::UNKNOWN_FORMAT;
  if (create_table.storage_format.length() == 0) {
    storage_format = StorageFormat::ROW_FORMAT;
  } else {
    storage_format = get_storage_format(create_table.storage_format.c_str());
  }
  if (storage_format == StorageFormat::UNKNOWN_FORMAT) {
    return RC::INVALID_ARGUMENT;
  }
  // 处理带有 SELECT 的 CREATE TABLE
  if (!select_sql.expressions.empty()) {
    Stmt *select_stmt = nullptr;
    std::vector<AttrInfoSqlNode> attr_infos;

    RC rc = SelectStmt::create(db, select_sql, select_stmt);
    if (RC::SUCCESS != rc) {
      LOG_WARN("failed to create select_stmt when create_table_select, rc=%s", strrc(rc));
      return rc;
    }

  // 填充 attr_infos
  for (const auto &attr_expr : dynamic_cast<SelectStmt *>(select_stmt)->query_expressions()) {
    AttrInfoSqlNode attr_info;
    populateAttrInfo(attr_expr, attr_info);
    attr_infos.emplace_back(attr_info);
  }
  if (0 != create_table.attr_infos.size()) {
    if (attr_infos.size() != create_table.attr_infos.size()) {
      LOG_ERROR("field size mismatch with output column size of select_stmt");
      return RC::INVALID_ARGUMENT;
    }
    for (size_t i = 0; i < attr_infos.size(); i++) {
      if (attr_infos[i].type != create_table.attr_infos[i].type) {
        LOG_ERROR("create_table info mismatch with sub_query");
        return RC::INVALID_ARGUMENT;
      }
    }
      // 指定了列属性、带select
      stmt = new CreateTableStmt(db,create_table.relation_name, create_table.attr_infos,storage_format, select_stmt);
    } else {
      // 未指定列属性、带select
      stmt = new CreateTableStmt(db,create_table.relation_name, attr_infos, storage_format, select_stmt);
    }
  } else {
    // 指定了列属性、不带select
    stmt = new CreateTableStmt(db,create_table.relation_name, create_table.attr_infos, storage_format,nullptr);

  }
  sql_debug("create table statement: table name %s", create_table.relation_name.c_str());
  return RC::SUCCESS;

}

StorageFormat CreateTableStmt::get_storage_format(const char *format_str) {
  StorageFormat format = StorageFormat::UNKNOWN_FORMAT;
  if (0 == strcasecmp(format_str, "ROW")) {
    format = StorageFormat::ROW_FORMAT;
  } else if (0 == strcasecmp(format_str, "PAX")) {
    format = StorageFormat::PAX_FORMAT;
  } else {
    format = StorageFormat::UNKNOWN_FORMAT;
  }
  return format;
}
// 将字段属性初始化分离成独立函数
void CreateTableStmt::populateAttrInfo(const std::unique_ptr<Expression> &attr_expr, AttrInfoSqlNode &attr_info){
  // 获取字段名
  attr_info.name = (attr_expr->alias().length() != 0) ?
      attr_expr->alias().substr(attr_expr->alias().find('.') + 1) :
      attr_expr->name();

  // 设置字段类型
  attr_info.type = attr_expr->value_type();
  // 根据字段类型设置长度
  attr_info.length =  dynamic_cast<FieldExpr *>(attr_expr.get())->get_field_meta().len();
  // 检查是否包含可空的子表达式
  // 遍历子表达式，有nullable的FieldExpr时，才允许为NULL
  bool nullable = false;
  auto check_expr_nullable = [&nullable](Expression *expr) {
    if (ExprType::FIELD == expr->type()) {
      FieldMeta field = dynamic_cast<FieldExpr*>(expr)->get_field_meta();
      if (field.nullable()) {
        nullable = true;
      }
    }
  };
  attr_expr->traverse(check_expr_nullable);
  attr_info.nullable = nullable;
}
