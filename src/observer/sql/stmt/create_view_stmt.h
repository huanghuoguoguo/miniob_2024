//
// Created by glwuy on 24-10-29.
//

#ifndef CREATE_VIEW_STMT_H
#define CREATE_VIEW_STMT_H


#include <string>
#include <vector>

#include "select_stmt.h"
#include "sql/stmt/stmt.h"
class Db;
class Field;
/**
 * @brief 表示创建视图的语句
 * @ingroup Statement
 * @details 虽然解析成了stmt，但是与原始的SQL解析后的数据也差不多
 */
class CreateViewStmt : public Stmt
{
public:
  CreateViewStmt(const std::string &view_name, SelectStmt *select_stmt);
  virtual   ~CreateViewStmt() = default;
  static RC create(Db *db, const CreateViewSqlNode &create_table, Stmt *&stmt);

public:
  std::string view_name() const { return view_name_; }

  StmtType type() const override { return StmtType::CREATE_VIEW; }

private:
  std::string    view_name_;
  SelectStmt *   select_stmt_; // SelectStmt里存的是指针，不便于落盘管理
public:
  SelectStmt * select_stmt()
  {
    return select_stmt_;
  }
};


#endif //CREATE_VIEW_STMT_H