//
// Created by glwuy on 24-10-19.
//

#include "arithmetic_simplification_rule.h"
#include "common/log/log.h"
#include "sql/expr/expression.h"

RC ArithmeticSimplificationRule::rewrite(std::unique_ptr<Expression> &expr, bool &change_made)
{
  RC rc = RC::SUCCESS;

  change_made = false;

  if (expr->type() == ExprType::CAST) {
    auto  cast_expr        = static_cast<CastExpr *>(expr.get());
    auto &child_expression = cast_expr->child();
    // 如果孩子直接是值的话，替换。
    if (child_expression->type() == ExprType::VALUE) {
      expr.swap(child_expression);
      change_made = true;
      return RC::SUCCESS;
    }
  }

  if (expr->type() == ExprType::ARITHMETIC) {
    auto  arithmetic_expr = static_cast<ArithmeticExpr *>(expr.get());
    auto &left            = arithmetic_expr->left();
    auto &right           = arithmetic_expr->right();

    // 如果二者都是值，可以优先直接将其计算出来。
    if (left->type() == ExprType::VALUE && right->type() == ExprType::VALUE) {
      Value value;
      RC    sub_rc = arithmetic_expr->try_get_value(value);
      if (sub_rc == RC::SUCCESS) {
        std::unique_ptr<Expression> new_expr(new ValueExpr(value));
        expr.swap(new_expr);
        change_made = true;
        LOG_TRACE("arithmetic expression is simplified");
      }else {
        value.set_boolean(false);
        std::unique_ptr<Expression> new_expr(new ValueExpr(value));
        expr.swap(new_expr);
        change_made = true;
      }
    }
  }
  return rc;
}