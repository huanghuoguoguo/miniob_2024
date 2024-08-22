//
// Created by Bob on 24-8-20.
//

#include "drop_table_stmt.h"
#include "event/sql_debug.h"

RC DropTableStmt::create(Db *db, const DropTableSqlNode &drop_table, Stmt *&stmt) {


  stmt = new DropTableStmt(drop_table.relation_name);
  sql_debug("drop table statement: table name %s", drop_table.relation_name.c_str());
  return RC::SUCCESS;
}
