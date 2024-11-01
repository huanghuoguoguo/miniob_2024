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
// Created by Wangyunlai on 2022/12/30.
//

#include "sql/optimizer/predicate_pushdown_rewriter.h"

#include <sql/operator/join_logical_operator.h>
#include <sql/operator/physical_operator.h>
#include <sql/operator/predicate_logical_operator.h>
#include <storage/table/view.h>

#include "common/log/log.h"
#include "sql/expr/expression.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"

class View;

RC PredicatePushdownRewriter::try_rewriter_table_get(
    std::unique_ptr<Expression> &predicate_expr, bool &change_made, std::unique_ptr<LogicalOperator> &child_oper)
{
  RC   rc             = RC::SUCCESS;
  auto table_get_oper = static_cast<TableGetLogicalOperator *>(child_oper.get());

  std::vector<std::unique_ptr<Expression>> pushdown_exprs;
  rc = get_exprs_can_pushdown(predicate_expr, pushdown_exprs, table_get_oper);
  if (rc == RC::UNIMPLEMENTED) {
    rc = RC::SUCCESS;
  }
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get exprs can pushdown. rc=%s", strrc(rc));
    return rc;
  }

  if (!predicate_expr || is_empty_predicate(predicate_expr)) {
    // 所有的表达式都下推到了下层算子
    // 这个predicate operator其实就可以不要了。但是这里没办法删除，弄一个空的表达式吧
    LOG_TRACE("all expressions of predicate operator were pushdown to table get operator, then make a fake one");

    Value value((bool)true);
    predicate_expr = std::unique_ptr<Expression>(new ValueExpr(value));
  }

  if (!pushdown_exprs.empty()) {
    change_made = true;
    table_get_oper->set_predicates(std::move(pushdown_exprs));
  }
  return rc;
}

RC PredicatePushdownRewriter::rewrite(std::unique_ptr<LogicalOperator> &oper, bool &change_made)
{
  RC rc = RC::SUCCESS;
  if (oper->type() != LogicalOperatorType::PREDICATE) {
    return rc;
  }

  if (oper->children().size() != 1) {
    return rc;
  }

  /**
   * 不管是什么，都尝试下沉。
   */
  std::unique_ptr<LogicalOperator> &child_oper = oper->children().front();
  if (child_oper->type() == LogicalOperatorType::TABLE_GET) {
    rc = try_rewriter_table_get(oper->expressions().front(), change_made, child_oper);
    if (rc != RC::SUCCESS)
      return rc;
  } else {
    // 对每个表达式，都遍历所有子孩子，找寻是否可以将其推入。
    std::vector<std::unique_ptr<Expression>> &predicate_oper_exprs = oper->expressions();
    for (auto &predicate_expr : predicate_oper_exprs) {
      // 对每一个表达式，都将其尝试下沉。
      rc = try_rewrite_join(oper, predicate_expr, change_made);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get exprs can pushdown. rc=%s", strrc(rc));
        return rc;
      }
    }

    if (predicate_oper_exprs.size() > 1) {
      // 预先创建 true 表达式，避免在循环中多次创建
      Value                  value((bool)true);
      unique_ptr<Expression> true_expression(new ValueExpr(value));

      for (auto iter = predicate_oper_exprs.begin(); iter != predicate_oper_exprs.end(); ++iter) {
        if (iter->get()->type() == ExprType::VALUE) {
          if (iter->get()->equal(*true_expression))
            predicate_oper_exprs.erase(iter);
        }
      }

      // 如果列表为空，则添加一个 true 表达式
      if (predicate_oper_exprs.empty()) {
        predicate_oper_exprs.push_back(std::move(true_expression));
      }
    }

  }
  return rc;
}


RC PredicatePushdownRewriter::try_rewrite_join(std::unique_ptr<LogicalOperator> &oper,
    std::unique_ptr<Expression> &                                                predicate_expr,
    bool &                                                                       change_made
    )
{
  RC    rc        = RC::SUCCESS;
  auto &childrens = oper->children();
  for (auto &child : childrens) {
    // 如果孩子是table_get，则进行判断，是否可以进行下沉。
    if (child->type() == LogicalOperatorType::TABLE_GET) {
      rc = try_rewriter_table_get(predicate_expr, change_made, child);
      if (rc != RC::SUCCESS)
        return rc;
    } else {
      // 不是，继续下沉。
      rc = try_rewrite_join(child, predicate_expr, change_made);
      if (rc != RC::SUCCESS)
        return rc;
    }
  }
  return rc;
}


bool PredicatePushdownRewriter::is_empty_predicate(std::unique_ptr<Expression> &expr)
{
  bool bool_ret = false;
  if (!expr) {
    return true;
  }

  if (expr->type() == ExprType::CONJUNCTION) {
    ConjunctionExpr *conjunction_expr = static_cast<ConjunctionExpr *>(expr.get());
    if (conjunction_expr->children().empty()) {
      bool_ret = true;
    }
  }

  return bool_ret;
}

/**
 * 查看表达式是否可以直接下放到table get算子的filter
 * @param expr 是当前的表达式。如果可以下放给table get 算子，执行完成后expr就失效了
 * @param pushdown_exprs 当前所有要下放给table get 算子的filter。此函数执行多次，
 *                       pushdown_exprs 只会增加，不要做清理操作
 */
RC PredicatePushdownRewriter::get_exprs_can_pushdown(
    std::unique_ptr<Expression> &expr, std::vector<std::unique_ptr<Expression>> &pushdown_exprs,
    TableGetLogicalOperator *    table_get_oper)
{
  RC     rc    = RC::SUCCESS;
  Table *table = table_get_oper->table();
  if (View *v = dynamic_cast<View *>(table)) {
    // view不下推。
    LOG_INFO("view not down:%s",v->name());
    return rc;
  }

  if (expr->type() == ExprType::CONJUNCTION) {
    ConjunctionExpr *conjunction_expr = static_cast<ConjunctionExpr *>(expr.get());
    // 或 操作的比较，太复杂，现在不考虑
    if (conjunction_expr->conjunction_type() == ConjunctionExpr::Type::OR) {
      LOG_WARN("unsupported or operation");
      // or不下推。
      rc = RC::UNIMPLEMENTED;
      return rc;
    }

    std::vector<std::unique_ptr<Expression>> &child_exprs = conjunction_expr->children();
    for (auto iter = child_exprs.begin(); iter != child_exprs.end();) {
      // 对每个子表达式，判断是否可以下放到table get 算子
      // 如果可以的话，就从当前孩子节点中删除他
      rc = get_exprs_can_pushdown(*iter, pushdown_exprs, table_get_oper);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get pushdown expressions. rc=%s", strrc(rc));
        return rc;
      }

      if (!*iter) {
        child_exprs.erase(iter);
      } else {
        ++iter;
      }
    }
  } else if (expr->type() == ExprType::COMPARISON) {
    // 如果是比较操作，并且比较的左边或右边是表某个列值，那么就下推下去
    auto comparison_expr = static_cast<ComparisonExpr *>(expr.get());

    std::unique_ptr<Expression> &left_expr  = comparison_expr->left();
    std::unique_ptr<Expression> &right_expr = comparison_expr->right();

    // 比较操作的左右两边只要有一个是取列字段值的并且另一边也是取字段值或常量，就pushdown
    if (left_expr->type() != ExprType::FIELD && right_expr->type() != ExprType::FIELD) {
      return rc;
    }
    if (left_expr->type() != ExprType::FIELD && left_expr->type() != ExprType::VALUE &&
        right_expr->type() != ExprType::FIELD && right_expr->type() != ExprType::VALUE) {
      return rc;
    }

    // 表达式中的字段值和当前table匹配时才有效。 对以下的情况，只有其中一个为field，另一个为value，并且匹配段名的时候才有效。
    if (left_expr->type() == ExprType::FIELD) {
      auto left_field_expr = static_cast<FieldExpr *>(left_expr.get());
      if (0 != strcmp(left_field_expr->table_name(), table_get_oper->table()->table_meta().name()) || right_expr->type()
          != ExprType::VALUE) {
        return rc;
      }
    }
    if (right_expr->type() == ExprType::FIELD) {
      auto right_field_expr = static_cast<FieldExpr *>(right_expr.get());
      if (0 != strcmp(right_field_expr->table_name(), table_get_oper->table()->table_meta().name()) || left_expr->type()
          != ExprType::VALUE) {
        return rc;
      }
    }

    pushdown_exprs.emplace_back(std::move(expr));
  }
  return rc;
}