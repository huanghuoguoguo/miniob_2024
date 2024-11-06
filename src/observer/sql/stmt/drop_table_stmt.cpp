//
// Created by 25689 on 2024/8/21.
//

#include "sql/stmt/drop_table_stmt.h"
#include "event/sql_debug.h"

RC DropTableStmt::create(Db *db, const DropTableSqlNode &drop_table, Stmt *&stmt)
{

  stmt = new DropTableStmt(drop_table.relation_name);
  return RC::SUCCESS;
}

