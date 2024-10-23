//
// Created by admin on 24-10-23.
//
#include <algorithm>

#include "common/log/log.h"
#include "sql/operator/order_by_physical_operator.h"
#include "sql/expr/expression_tuple.h"
#include "sql/expr/composite_tuple.h"

using namespace std;
using namespace common;


OrderByPhysicalOperator::OrderByPhysicalOperator(std::vector<Expression *> &&order_by_expressions,
                                                 std::vector<bool> &&order_by_directions)
    : order_by_expressions_(std::move(order_by_expressions)),
      order_by_directions_(std::move(order_by_directions)) {}

RC OrderByPhysicalOperator::open(Trx *trx) {
    RC rc = RC::SUCCESS;
    if (children_.size() != 1) {
        LOG_WARN("OrderByPhysicalOperator must have one child");
        return RC::INTERNAL;
    }
    if (RC::SUCCESS != (rc = children_[0]->open(trx))) {
        LOG_WARN("Child operator open failed!");
        return RC::INTERNAL;
    }
    rc = fetch_and_sort_tables();
    return rc;
}

RC OrderByPhysicalOperator::fetch_and_sort_tables() {
    LOG_WARN("Begin sorting");
    RC rc = RC::SUCCESS;

    std::vector<std::pair<std::vector<Value>, size_t>> pair_sort_table; // 要排序的内容
    std::vector<Value> row_values(order_by_expressions_.size()); // 缓存每一行

    // 获取所有行的值
    while (RC::SUCCESS == (rc = children_[0]->next())) {
        for (size_t i = 0; i < order_by_expressions_.size(); ++i) {
            Value cell;
            if (order_by_expressions_[i]->get_value(*children_[0]->current_tuple(), cell) != RC::SUCCESS) {
                LOG_WARN("Error retrieving value for sorting");
                return RC::INTERNAL;
            }
            row_values[i] = cell;
        }
        pair_sort_table.emplace_back(std::make_pair(row_values, pair_sort_table.size()));
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

    std::sort(pair_sort_table.begin(), pair_sort_table.end(), cmp);
    LOG_INFO("Sort Table Success");

    // 填充排序后的元组
    for (const auto &pair : pair_sort_table) {
        sorted_tuples_.emplace_back(new Tuple(pair.first)); // 假设有合适的构造函数
    }

    current_index_ = 0;
    return rc;
}

RC OrderByPhysicalOperator::next() {
    // 检查当前索引是否在有效范围内
    if (current_index_ < sorted_tuples_.size()) {
        // 获取当前元组
        Tuple *current = sorted_tuples_[current_index_++];
        // 将当前元组设置为返回的元组
        // 如果需要返回的元组是通过某种方式输出，可以添加一个成员变量来存储
        // 例如: returned_tuple_ = current;  // 假设有一个返回的元组指针成员

        // 如果没有其他成员变量用于存储返回的元组，直接返回 RC::SUCCESS
        return RC::SUCCESS;
    }
    return RC::RECORD_EOF; // 到达文件末尾
}

RC OrderByPhysicalOperator::close() {
    // 释放 sorted_tuples_ 中的每个 Tuple 的内存
    for (Tuple *tuple : sorted_tuples_) {
        delete tuple; // 假设需要手动释放内存
    }
    sorted_tuples_.clear(); // 清空容器
    current_index_ = 0; // 重置当前索引（可选）

    return RC::SUCCESS; // 返回成功状态
}

Tuple *OrderByPhysicalOperator::current_tuple() {
    if (current_index_ < sorted_tuples_.size()) {
        return sorted_tuples_[current_index_]; // 返回当前元组
    }
    return nullptr; // 索引无效时返回空指针
}