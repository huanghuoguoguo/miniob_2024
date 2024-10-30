//
// Created by glwuy on 24-10-29.
//

#include "create_view_stmt.h"

#include <sql/parser/expression_binder.h>

#include "sql/stmt/select_stmt.h"
#include "storage/db/db.h"
#include "storage/field/field.h"
#include "event/sql_debug.h"

CreateViewStmt::CreateViewStmt(const std::string &view_name,
    SelectStmt *                                  select_stmt)
  : view_name_(view_name)
{
  this->select_stmt_    = select_stmt;
}

RC CreateViewStmt::create(Db *db, const CreateViewSqlNode &create_view, Stmt *&stmt)
{
  RC rc = RC::SUCCESS;
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }
  if (create_view.select_sql_node == nullptr) {
    return RC::INVALID_ARGUMENT;
  }

  Stmt *sub_stmt = nullptr;
  rc             = SelectStmt::create(db, *create_view.select_sql_node, sub_stmt);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  // 获取querys字段。
  // 获取binder_context，将表指针全部获取。
  // 后续操作中，如果表大于两个，不允许修改，只允许查找。
  if (nullptr == sub_stmt) {
    return RC::INVALID_ARGUMENT;
  }
  SelectStmt *select_stmt = static_cast<SelectStmt *>(sub_stmt);

  stmt = new CreateViewStmt(create_view.view_name, select_stmt);

  sql_debug("create view statement: table name %s", create_view.view_name.c_str());

  return rc;
}