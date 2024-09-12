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

#pragma once

#include "common/rc.h"
#include "sql/stmt/stmt.h"
#include "filter_stmt.h"

class Table;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
    UpdateStmt() = default; //初始化stmt对象
    ~UpdateStmt() override; // 析构函数负责清理内存资源

    StmtType type() const override { return StmtType::UPDATE; } //返回update 表示这是一个update语句


public:
    static RC create(Db* db, const UpdateSqlNode& update_sql, Stmt*& stmt);

public:
    Table* table() const { return table_; } // 返回需要update的表
    const std::string& attribute_name() const { return attribute_name_; }
    const Value& value() const { return value_; }
    FilterStmt* filter_stmt() const { return filter_stmt_; } //返回where生成的FilterStmt指针
    std::unique_ptr<ComparisonExpr>& getComparisonExpr() { return comparisonExpr; } // 返回select语句的查询表达式

private:
    Table* table_ = nullptr;
    std::string attribute_name_;
    Value value_;
    FilterStmt* filter_stmt_ = nullptr;
    std::unique_ptr<ComparisonExpr> comparisonExpr = nullptr;
};
