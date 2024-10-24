//
// Created by admin on 24-10-23.
//
#include <algorithm>

#include "common/log/log.h"
#include "sql/operator/order_by_physical_operator.h"
#include "sql/expr/expression_tuple.h"
#include "sql/expr/tuple.h"

using namespace std;
using namespace common;


OrderByPhysicalOperator::OrderByPhysicalOperator(std::vector<Expression *> &&order_by_expressions,
                                                 std::vector<bool> &&order_by_directions)
    : order_by_expressions_(std::move(order_by_expressions)),
      order_by_directions_(std::move(order_by_directions)) {}

RC OrderByPhysicalOperator::open(Trx *trx) {
    RC rc = children_[0]->open(trx);  // 打开子算子
    if (OB_FAIL(rc)) {
        LOG_WARN("failed to open child operator. rc=%s", strrc(rc));
        return rc;
    }

    // 获取所有元组并进行排序
    rc = fetch_and_sort();
    if (OB_FAIL(rc)) {
        LOG_WARN("failed to fetch and sort tuples. rc=%s", strrc(rc));
        return rc;
    }

    current_index_ = 0;
    return rc;
}

RC OrderByPhysicalOperator::fetch_and_sort() {
    LOG_WARN("Begin sorting");
    RC rc = RC::SUCCESS;

    std::vector<std::pair<std::vector<Value>, size_t>> pair_sort_table; // 要排序的内容
    std::vector<Value> row_values(order_by_expressions_.size()); // 缓存每一行
    std::vector<std::unique_ptr<ValueListTuple>> child_tuples; // 用于存储所有元组

    // 获取所有行的值
    while (RC::SUCCESS == (rc = children_[0]->next())) {
        Tuple *child_tuple = children_[0]->current_tuple();
        if (child_tuple == nullptr) {
            LOG_WARN("Error retrieving child tuple");
            return RC::INTERNAL;
        }
        auto                          *order_by_evaluated_tuple = new ValueListTuple();
        rc = ValueListTuple::make(*child_tuple, *order_by_evaluated_tuple);
        if(OB_FAIL(rc)) {
            return rc;
        }
        child_tuples.emplace_back(order_by_evaluated_tuple);


        // 获取用于排序的值
        for (size_t i = 0; i < order_by_expressions_.size(); ++i) {
            Value cell;
            if (order_by_expressions_[i]->get_value(*child_tuple, cell) != RC::SUCCESS) {
                LOG_WARN("Error retrieving value for sorting");
                return RC::INTERNAL;
            }
            row_values[i] = cell;
        }

        // 将排序键和值的索引保存到排序表中
        pair_sort_table.emplace_back(row_values, child_tuples.size() - 1); // 使用 child_tuples 的索引
    }

    if (RC::RECORD_EOF != rc) {
        LOG_ERROR("Fetch Table Error In SortOperator. RC: %d", rc);
        return rc;
    }

    // 排序逻辑
    auto cmp = [this](const auto &a, const auto &b) {
        for (size_t i = 0; i < order_by_expressions_.size(); ++i) {
            const Value &cell_a = a.first[i];
            const Value &cell_b = b.first[i];
            if (cell_a.is_null() && !cell_b.is_null()) {
                return false; // cell_a 是 null，cell_b 不是，cell_a 排在后面
            }

            if (!cell_a.is_null() && cell_b.is_null()) {
                return true; // cell_b 是 null，cell_a 不是，cell_a 排在前面
            }

            int comparison = cell_a.compare(cell_b); // 使用 compare 函数进行比较
            if (comparison != 0) {
                return order_by_directions_[i] ? (comparison < 0) : (comparison > 0); // 根据排序方向返回
            }
        }
        return false; // 完全相同
    };

    // 排序
    std::sort(pair_sort_table.begin(), pair_sort_table.end(), cmp);
    LOG_INFO("Sort Table Success");

    // 根据排序后的顺序，填充 sorted_tuples_
    sorted_tuples_.clear(); // 清除之前的内容
    for (const auto &pair : pair_sort_table) {
        sorted_tuples_.emplace_back(std::move(child_tuples[pair.second]).release()); // 按排序后的顺序插入
    }

    current_index_ = 0;
    return RC::SUCCESS;
}

RC OrderByPhysicalOperator::next() {
    // 检查当前索引是否在有效范围内
    if (current_index_ >= sorted_tuples_.size()) {
        return RC::RECORD_EOF;
    }

    current_index_++;
    return RC::SUCCESS;
}


RC OrderByPhysicalOperator::close() {
    RC rc = children_[0]->close();
    if (OB_FAIL(rc)) {
        LOG_WARN("failed to close child operator.");
        return rc;
    }

    sorted_tuples_.clear();
    return RC::SUCCESS;
}

Tuple *OrderByPhysicalOperator::current_tuple() {
    if (current_index_ > 0 && current_index_ <= sorted_tuples_.size()) {
        return sorted_tuples_[current_index_ - 1];
    }
    return nullptr;
}