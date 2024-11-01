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
// Created by Wangyunlai on 2024/05/29.
//

#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/expression_binder.h"
#include "sql/expr/expression_iterator.h"

using namespace std;
using namespace common;

Table *BinderContext::find_table(const char *table_name) const
{
  // 先在 as_tables_ 中查找表别名
  auto alias_iter = as_tables_.find(table_name);
  if (alias_iter != as_tables_.end()) {
    return alias_iter->second; // 返回找到的 Table* 指针
  }
   alias_iter = as_parent_tables_.find(table_name);
  if (alias_iter != as_parent_tables_.end()) {
    return alias_iter->second; // 返回找到的 Table* 指针
  }

  // 如果 as_tables_ 中没有找到，再在 query_tables_ 中查找表名
  auto pred = [table_name](Table *table) { return 0 == strcasecmp(table_name, table->name()); };
  auto iter = ranges::find_if(query_tables_, pred);

  if (iter == query_tables_.end()) {
    return nullptr; // 如果找不到对应的表
  }

  return *iter;
}

Table *BinderContext::find_table_by_field(const char *field_name)
{
  // 先查询cur的。
  for(auto& table : cur_tables_) {
    const FieldMeta *field_meta = table->table_meta().field(field_name);
    if (nullptr != field_meta) {
      return table;
    }
  }
  for(auto& table : query_tables_) {
    const FieldMeta *field_meta = table->table_meta().field(field_name);
    if (nullptr != field_meta) {
      return table;
    }
  }
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
static void wildcard_fields(Table *table, vector<unique_ptr<Expression>> &expressions,bool is_muti)
{
  const TableMeta &table_meta = table->table_meta();
  const int        field_num  = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    Field      field(table, table_meta.field(i));
    FieldExpr *field_expr = new FieldExpr(field);
    string      field_name = field.field_name();
    if(is_muti) {
      field_name = string(table->table_meta().name())+"."+field_name;
    }
    field_expr->set_name(field_name.c_str());
    expressions.emplace_back(field_expr);
  }
}

RC ExpressionBinder::bind_expression(unique_ptr<Expression> &expr, vector<unique_ptr<Expression>> &bound_expressions)
{
  if (nullptr == expr) {
    return RC::SUCCESS;
  }

  switch (expr->type()) {
    case ExprType::STAR: {
      return bind_star_expression(expr, bound_expressions);
    } break;

    case ExprType::UNBOUND_FIELD: {
      return bind_unbound_field_expression(expr, bound_expressions);
    } break;

    case ExprType::UNBOUND_AGGREGATION: {
      return bind_aggregate_expression(expr, bound_expressions);
    } break;

    case ExprType::FIELD: {
      return bind_field_expression(expr, bound_expressions);
    } break;

    case ExprType::VALUE: {
      return bind_value_expression(expr, bound_expressions);
    } break;

    case ExprType::CAST: {
      return bind_cast_expression(expr, bound_expressions);
    } break;

    case ExprType::COMPARISON: {
      return bind_comparison_expression(expr, bound_expressions);
    } break;

    case ExprType::CONJUNCTION: {
      return bind_conjunction_expression(expr, bound_expressions);
    } break;

    case ExprType::ARITHMETIC: {
      return bind_arithmetic_expression(expr, bound_expressions);
    } break;

    case ExprType::AGGREGATION: {
      ASSERT(false, "shouldn't be here");
    } break;

    case ExprType::SUB_QUERY: {
      return bind_sub_expression(expr, bound_expressions);
    }
    case ExprType::FUNCTION: {
      return bind_function_expression(expr, bound_expressions);
    }
    default: {
      LOG_WARN("unknown expression type: %d", static_cast<int>(expr->type()));
      return RC::INTERNAL;
    }
  }
  return RC::INTERNAL;
}

RC ExpressionBinder::bind_star_expression(
    unique_ptr<Expression> &expr, vector<unique_ptr<Expression>> &bound_expressions)
{
  if (nullptr == expr) {
    return RC::SUCCESS;
  }

  auto star_expr = static_cast<StarExpr *>(expr.get());

  if(!star_expr->alias().empty()) {
    return RC::INTERNAL;
  }

  vector<Table *> tables_to_wildcard;

  const char *table_name = star_expr->table_name();
  //若 table_name 不是空且不等于 "*", 则表示只从指定的 table_name 表中获取字段。
  if (!is_blank(table_name) && 0 != strcmp(table_name, "*")) {
    Table *table = context_.find_table(table_name);
    if (nullptr == table) {
      LOG_INFO("no such table in from list: %s", table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    tables_to_wildcard.push_back(table);
  } else {
    //处理未指定表名的情况:
    const vector<Table *> &all_tables = context_.cur_tables();
    // 判断是否存在多个表 用于判断select * from t1,t2
    // if (all_tables.size() > 1) {
    //   LOG_WARN("SELECT * is not allowed for multiple tables.");
    //   return RC::INTERNAL;
    // }
    tables_to_wildcard.insert(tables_to_wildcard.end(), all_tables.begin(), all_tables.end());
  }


  for (Table *table : tables_to_wildcard) {
    wildcard_fields(table, bound_expressions, tables_to_wildcard.size() > 1);
  }

  return RC::SUCCESS;
}

RC ExpressionBinder::bind_unbound_field_expression(
    unique_ptr<Expression> &expr, vector<unique_ptr<Expression>> &bound_expressions)
{
  if (nullptr == expr) {
    return RC::SUCCESS;
  }
  auto unbound_field_expr = static_cast<UnboundFieldExpr *>(expr.get());

  const char *table_name = unbound_field_expr->table_name();
  const char *field_name = unbound_field_expr->field_name();
 std::string field_as_name = unbound_field_expr->alias();
  Table *table = nullptr;
  if (is_blank(table_name)) {
    // if (context_.query_tables().size() != 1) {
    //   LOG_INFO("cannot determine table for field: %s", field_name);
    //   return RC::SCHEMA_TABLE_NOT_EXIST;
    // }
    // 循环table。应该只能找到当前table，暂时先找全部的table。

    Table *find_table_by_field = context_.find_table_by_field(field_name);
    if (find_table_by_field != nullptr) {
      table = find_table_by_field;
    } else {
      table = *context_.query_tables().begin();
    }


  } else {
    table = context_.find_table(table_name);
    // const FieldMeta *field_meta = table->table_meta().field(field_name);
    // context_.add_as_field(field_as_name, field_meta);
    if (nullptr == table) {
      LOG_INFO("no such table in from list: %s", table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
  }
  if(context_.is_single()) {
    std::vector<Table *>& tables = context_.cur_tables();
    bool t = false;
    for(auto& table_ : tables) {
      if(table == table_) {
        t = true;
      }
    }
    // 如果已经是非独立的了，就不需要再判断了。
    context_.is_single(t);
  }

  if (0 == strcmp(field_name, "*")) {
    wildcard_fields(table, bound_expressions, false);
  } else {
    // const FieldMeta* field_meta = context_.get_as_field_meta(field_name); // 先尝试从上下文获取 field_meta
    //
    // // 检查 field_meta 是否为 nullptr
    // if (nullptr == field_meta) {
    //   // 如果未找到，从 table 的元数据获取
    //   field_meta = table->table_meta().field(field_name);
    //
    //   // 将找到的 field_meta 加入到上下文
    //   context_.add_as_field(field_as_name, field_meta);
    // }
    const FieldMeta* field_meta = table->table_meta().field(field_name);
    // 检查 field_meta 是否仍为 nullptr
    if (nullptr == field_meta) {
      LOG_INFO("no such field in table: %s.%s", table_name, field_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    Field      field(table, field_meta);
    FieldExpr *field_expr = new FieldExpr(field);
    string     name       = string(table->table_meta().name()) + "." + string(field.field_name());
    if(context_.query_tables().size() == 1) {
      name = string(field.field_name());
    }

    field_expr->set_name(name);
    field_expr->set_alias(field_as_name);
    bound_expressions.emplace_back(field_expr);
  }

  return RC::SUCCESS;
}

RC ExpressionBinder::bind_field_expression(
    unique_ptr<Expression> &field_expr, vector<unique_ptr<Expression>> &bound_expressions)
{
  bound_expressions.emplace_back(std::move(field_expr));
  return RC::SUCCESS;
}

RC ExpressionBinder::bind_value_expression(
    unique_ptr<Expression> &value_expr, vector<unique_ptr<Expression>> &bound_expressions)
{
  bound_expressions.emplace_back(std::move(value_expr));
  return RC::SUCCESS;
}

RC ExpressionBinder::bind_cast_expression(
    unique_ptr<Expression> &expr, vector<unique_ptr<Expression>> &bound_expressions)
{
  if (nullptr == expr) {
    return RC::SUCCESS;
  }

  auto cast_expr = static_cast<CastExpr *>(expr.get());

  vector<unique_ptr<Expression>> child_bound_expressions;
  unique_ptr<Expression>        &child_expr = cast_expr->child();

  RC rc = bind_expression(child_expr, child_bound_expressions);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  if (child_bound_expressions.size() != 1) {
    LOG_WARN("invalid children number of cast expression: %d", child_bound_expressions.size());
    return RC::INVALID_ARGUMENT;
  }

  unique_ptr<Expression> &child = child_bound_expressions[0];
  if (child.get() == child_expr.get()) {
    return RC::SUCCESS;
  }

  child_expr.reset(child.release());
  bound_expressions.emplace_back(std::move(expr));
  return RC::SUCCESS;
}

RC ExpressionBinder::bind_comparison_expression(
    unique_ptr<Expression> &expr, vector<unique_ptr<Expression>> &bound_expressions)
{
  if (nullptr == expr) {
    return RC::SUCCESS;
  }

  auto comparison_expr = static_cast<ComparisonExpr *>(expr.get());

  vector<unique_ptr<Expression>> child_bound_expressions;
  unique_ptr<Expression>        &left_expr  = comparison_expr->left();
  unique_ptr<Expression>        &right_expr = comparison_expr->right();

  RC rc = bind_expression(left_expr, child_bound_expressions);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  if (child_bound_expressions.size() != 1) {
    LOG_WARN("invalid left children number of comparison expression: %d", child_bound_expressions.size());
    return RC::INVALID_ARGUMENT;
  }

  unique_ptr<Expression> &left = child_bound_expressions[0];
  if (left.get() != left_expr.get()) {
    left_expr.reset(left.release());
  }

  child_bound_expressions.clear();
  rc = bind_expression(right_expr, child_bound_expressions);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  if (child_bound_expressions.size() != 1) {
    LOG_WARN("invalid right children number of comparison expression: %d", child_bound_expressions.size());
    return RC::INVALID_ARGUMENT;
  }

  unique_ptr<Expression> &right = child_bound_expressions[0];
  if (right.get() != right_expr.get()) {
    right_expr.reset(right.release());
  }

  bound_expressions.emplace_back(std::move(expr));
  return RC::SUCCESS;
}

RC ExpressionBinder::bind_conjunction_expression(
    unique_ptr<Expression> &expr, vector<unique_ptr<Expression>> &bound_expressions)
{
  if (nullptr == expr) {
    return RC::SUCCESS;
  }

  auto conjunction_expr = static_cast<ConjunctionExpr *>(expr.get());

  vector<unique_ptr<Expression>>  child_bound_expressions;
  vector<unique_ptr<Expression>> &children = conjunction_expr->children();

  for (unique_ptr<Expression> &child_expr : children) {
    child_bound_expressions.clear();

    RC rc = bind_expression(child_expr, child_bound_expressions);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    if (child_bound_expressions.size() != 1) {
      LOG_WARN("invalid children number of conjunction expression: %d", child_bound_expressions.size());
      return RC::INVALID_ARGUMENT;
    }

    unique_ptr<Expression> &child = child_bound_expressions[0];
    if (child.get() != child_expr.get()) {
      child_expr.reset(child.release());
    }
  }

  bound_expressions.emplace_back(std::move(expr));

  return RC::SUCCESS;
}

RC ExpressionBinder::bind_arithmetic_expression(
    unique_ptr<Expression> &expr, vector<unique_ptr<Expression>> &bound_expressions)
{
  if (nullptr == expr) {
    return RC::SUCCESS;
  }

  auto arithmetic_expr = static_cast<ArithmeticExpr *>(expr.get());

  vector<unique_ptr<Expression>> child_bound_expressions;
  unique_ptr<Expression>        &left_expr  = arithmetic_expr->left();
  unique_ptr<Expression>        &right_expr = arithmetic_expr->right();

  if (arithmetic_expr->isNegative()) {
    if (left_expr->type() == ExprType::VALUE) {
      Value value;
      RC    sub_rc = arithmetic_expr->try_get_value(value);
      if (sub_rc == RC::SUCCESS) {
        std::unique_ptr<Expression> new_expr(new ValueExpr(value));
        expr.swap(new_expr);
        return RC::SUCCESS;
      }
    }
  }

  RC rc = bind_expression(left_expr, child_bound_expressions);
  if (OB_FAIL(rc)) {
    return rc;
  }

  if (!arithmetic_expr->isNegative() && child_bound_expressions.size() > 1) {
    LOG_WARN("invalid left children number of comparison expression: %d", child_bound_expressions.size());
    return RC::INVALID_ARGUMENT;
  }

  if (!child_bound_expressions.empty()) {
    unique_ptr<Expression> &left = child_bound_expressions[0];
    if (left.get() != left_expr.get()) {
      left_expr.reset(left.release());
    }
  }

  child_bound_expressions.clear();
  rc = bind_expression(right_expr, child_bound_expressions);
  if (OB_FAIL(rc)) {
    return rc;
  }

  if (!arithmetic_expr->isNegative() && child_bound_expressions.size() > 1) {
    LOG_WARN("invalid right children number of comparison expression: %d", child_bound_expressions.size());
    return RC::INVALID_ARGUMENT;
  }

  if (!child_bound_expressions.empty()) {
    unique_ptr<Expression> &right = child_bound_expressions[0];
    if (right.get() != right_expr.get()) {
      right_expr.reset(right.release());
    }
  }

  bound_expressions.emplace_back(std::move(expr));
  return RC::SUCCESS;
}

RC check_aggregate_expression(AggregateExpr &expression)
{
  // 必须有一个子表达式
  Expression *child_expression = expression.child().get();
  if (nullptr == child_expression) {
    LOG_WARN("child expression of aggregate expression is null");
    return RC::INVALID_ARGUMENT;
  }

  // 校验数据类型与聚合类型是否匹配
  AggregateExpr::Type aggregate_type   = expression.aggregate_type();
  AttrType            child_value_type = child_expression->value_type();
  switch (aggregate_type) {
    case AggregateExpr::Type::SUM:
    case AggregateExpr::Type::AVG: {
      // 仅支持数值类型
      if (child_value_type != AttrType::INTS && child_value_type != AttrType::FLOATS) {
        LOG_WARN("invalid child value type for aggregate expression: %d", static_cast<int>(child_value_type));
        return RC::INVALID_ARGUMENT;
      }
    } break;

    case AggregateExpr::Type::COUNT:
    case AggregateExpr::Type::MAX:
    case AggregateExpr::Type::MIN: {
      // 任何类型都支持
    } break;
  }

  // 子表达式中不能再包含聚合表达式
  function<RC(std::unique_ptr<Expression>&)> check_aggregate_expr = [&](unique_ptr<Expression> &expr) -> RC {
    RC rc = RC::SUCCESS;
    if (expr->type() == ExprType::AGGREGATION) {
      LOG_WARN("aggregate expression cannot be nested");
      return RC::INVALID_ARGUMENT;
    }
    rc = ExpressionIterator::iterate_child_expr(*expr, check_aggregate_expr);
    return rc;
  };

  RC rc = ExpressionIterator::iterate_child_expr(expression, check_aggregate_expr);

  return rc;
}

RC ExpressionBinder::bind_aggregate_expression(
    unique_ptr<Expression> &expr, vector<unique_ptr<Expression>> &bound_expressions)
{
  if (nullptr == expr) {
    return RC::SUCCESS;
  }
  auto func_expr = static_cast<FunctionExpr *>(expr.get());

  string              func_name = func_expr->get_func_name();
  AggregateExpr::Type aggregate_type;
  RC                  rc = AggregateExpr::type_from_string(func_name.c_str(), aggregate_type);
  if (OB_FAIL(rc)) {
    return rc;
  }
  // 到这里能够确定他是聚合函数了，判断是不是只有一个字段。
  if (func_expr->params().size() != 1) {
    return RC::INVALID_ARGUMENT;
  }

  unique_ptr<Expression> &       child_expr = func_expr->params().front();
  vector<unique_ptr<Expression>> child_bound_expressions;

  if (child_expr->type() == ExprType::STAR && aggregate_type == AggregateExpr::Type::COUNT) {
    ValueExpr *value_expr = new ValueExpr(Value(1));
    child_expr.reset(value_expr);
  } else {
    rc = bind_expression(child_expr, child_bound_expressions);
    if (OB_FAIL(rc)) {
      return rc;
    }

    if (child_bound_expressions.size() != 1) {
      LOG_WARN("invalid children number of aggregate expression: %d", child_bound_expressions.size());
      return RC::INVALID_ARGUMENT;
    }

    if (child_bound_expressions[0].get() != child_expr.get()) {
      child_expr.reset(child_bound_expressions[0].release());
    }
  }

  auto aggregate_expr = make_unique<AggregateExpr>(aggregate_type, std::move(child_expr));
  aggregate_expr->set_name(func_expr->name());
  rc = check_aggregate_expression(*aggregate_expr);
  if (OB_FAIL(rc)) {
    return rc;
  }

  bound_expressions.emplace_back(std::move(aggregate_expr));
  return RC::SUCCESS;
}

RC ExpressionBinder::bind_condition_expression(std::vector<ConditionSqlNode>& condition_sql_nodes)
{
  RC rc = RC::SUCCESS;
  if(condition_sql_nodes.empty()) {
    return rc;
  }
  // 将表达式绑定。
  vector<unique_ptr<Expression>> where_expressions;
  for (ConditionSqlNode &expression : condition_sql_nodes) {
    where_expressions.clear();
    std::unique_ptr<Expression> left(expression.left_expr);
    RC                          rc = this->bind_expression(left, where_expressions);
    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }

    if(!where_expressions.empty()) {
      expression.left_expr = where_expressions[0].release();
    }else {
      expression.left_expr = left.release();
    }
    where_expressions.clear();

    std::unique_ptr<Expression> right(expression.right_expr);
    rc            = this->bind_expression(right, where_expressions);
    if(!where_expressions.empty()) {
      expression.right_expr = where_expressions[0].release();
    }else {
      expression.right_expr = right.release();
    }
    where_expressions.clear();

    if (OB_FAIL(rc)) {
      LOG_INFO("bind expression failed. rc=%s", strrc(rc));
      return rc;
    }
  }
  return rc;
}
RC ExpressionBinder::bind_sub_expression(
      std::unique_ptr<Expression> &expr, std::vector<std::unique_ptr<Expression>> &bound_expressions)
{
  RC rc = RC::SUCCESS;
  Stmt         *stmt           = nullptr;
  SubQueryExpr *sub_query_expr = static_cast<SubQueryExpr *>(expr.release());
  if (sub_query_expr->select_sql_node() != nullptr) {
    BinderContext * sub_context = new BinderContext();
    sub_context->db(this->context_.db());
    //把外部的别名指针对应表传入子查询
    for(auto as :this->context_.query_as_tables()) {
      sub_context->add_as_parent_table(as.first,as.second);
    }
    for(auto as :this->context_.query_as_parent_tables()) {
      sub_context->add_as_parent_table(as.first,as.second);
    }


    // 将当前的所有table放入下一层。
    for(auto& table :context_.query_tables()) {
      sub_context->add_table(table);
    }

    // 如果存在sqlnode。
    sub_query_expr->select_sql_node()->binder_context = sub_context;
    rc = SelectStmt::create(this->context_.db(), *sub_query_expr->select_sql_node(), stmt);
    sub_query_expr->set_select_stmt(static_cast<SelectStmt *>(stmt));
    // 如果sub_context不独立，那么当前stmt也不独立。
    if(!sub_context->is_single()) {
      this->context_.is_single(false);
    }
  } else {
    std::vector<std::unique_ptr<Expression>> * expressions = sub_query_expr->values();
    if(expressions != nullptr) {
      // 值定义。
      std::vector<std::unique_ptr<Expression>>* bound_values = new std::vector<std::unique_ptr<Expression>>();
      for(size_t i = 0; i < expressions->size(); ++i) {
        bind_expression(expressions->at(i), *bound_values);
      }
      // 检查是否存在不一样的。
      if (!bound_values->empty()) {
        for (size_t i = 1; i < bound_values->size(); ++i) {
          if (bound_values->at(i)->type() != bound_values->at(i - 1)->type()) {
            return RC::INVALID_ARGUMENT;
          }
        }
      } else {
        // 是否添加一个null。
        bound_values->emplace_back(new ValueExpr(Value()));
      }
        // 直接加入list
      for (size_t i = 0; i < bound_values->size(); ++i) {
        Value *v  = new Value();
        RC     rc = static_cast<ValueExpr *>(bound_values->at(i).get())->try_get_value(*v);
        if (OB_FAIL(rc)) {
          return rc;
        }
        sub_query_expr->add_value(v);
      }
    }
  }
  bound_expressions.emplace_back(sub_query_expr);
  return rc;
}

RC ExpressionBinder::bind_function_expression(
    std::unique_ptr<Expression> &expr, std::vector<std::unique_ptr<Expression>> &bound_expressions)
{
  // 传输进来的是类似于func(params...)的类型。在这里进行一个分类，如果是聚合函数，直接调用绑定聚合函数。
  RC rc = RC::SUCCESS;
  // 优先绑定聚合函数。
  rc = bind_aggregate_expression(expr, bound_expressions);
  if (rc == RC::SUCCESS) {
    return rc;
  }
  if (rc != RC::UNKNOWN_FUNC) {
    return rc;
  }
  // 绑定其他函数。
  auto func_expr = static_cast<FunctionExpr *>(expr.get());

  string             func_name = func_expr->get_func_name();
  FunctionExpr::Type func_type;
  rc = FunctionExpr::type_from_string(func_name.c_str(), func_type);
  if (rc != RC::SUCCESS) {
    return rc;
  }
  // 这里粗糙的设定参数只能为两个。
  if (func_expr->params().size() != 2) {
    return RC::INVALID_ARGUMENT;
  }
  // 绑定函数类型
  func_expr->func_type(func_type);
  // 将param都绑定
  vector<unique_ptr<Expression>> child_bound_expressions;
  for (auto &param : func_expr->params()) {
    rc = bind_expression(param, child_bound_expressions);
  }
  func_expr->params().swap(child_bound_expressions);
  // 对每个params检查，这里偷懒，必须是vector的，如果是char的，转为vector。
  for (auto &param : func_expr->params()) {
    // 如果是字段，检查是不是vector
    if (param->type() == ExprType::FIELD) {
      FieldExpr *field_expr = static_cast<FieldExpr *>(param.get());
      if (field_expr->field().meta()->type() != AttrType::VECTORS) {
        return RC::INVALID_ARGUMENT;
      }
    } else if (param->type() == ExprType::VALUE) {
      ValueExpr *value_expr = static_cast<ValueExpr *>(param.get());
      if (value_expr->value_type() == AttrType::CHARS) {
        // 直接将其转为vector
        Value v;
        DataType::type_instance(AttrType::CHARS)->cast_to(value_expr->get_value(), AttrType::VECTORS, v);

        unique_ptr<Expression> e = make_unique<ValueExpr>(v);
        param.swap(e);
      }
      if (param->value_type() != AttrType::VECTORS) {
        return RC::INVALID_ARGUMENT;
      }
    } else {
      // 可能是表达式？
      if (param->value_type() != AttrType::VECTORS) {
        return RC::INVALID_ARGUMENT;
      }
    }
  }

  bound_expressions.emplace_back(std::move(expr));

  return rc;
}