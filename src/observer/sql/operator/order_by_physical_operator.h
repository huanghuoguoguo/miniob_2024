//
// Created by admin on 24-10-23.
//
#include "sql/operator/physical_operator.h"

class OrderByPhysicalOperator : public PhysicalOperator {
public:
    OrderByPhysicalOperator(std::vector<Expression *> &&order_by_expressions,
                            std::vector<bool> &&order_by_directions);
    virtual ~OrderByPhysicalOperator() = default;

    PhysicalOperatorType type() const override { return PhysicalOperatorType::ORDER_BY; }

    RC open(Trx *trx) override;

    RC fetch_and_sort();

    RC next() override;
    RC close() override;

    Tuple *current_tuple() override;

private:
    std::vector<Expression *> order_by_expressions_; // 排序表达式
    std::vector<bool> order_by_directions_; // 排序方向，true 为升序，false 为降序
    std::vector<Tuple *> sorted_tuples_; // 存储排序后的元组
    size_t current_index_ = 0; // 当前处理的元组索引
};