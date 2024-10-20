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

#include "sql/expr/aggregator.h"
#include "common/log/log.h"

#include <algorithm>
#include <bits/ranges_algo.h>

RC SumAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED || value_.attr_type() == AttrType::NULL_) {
    value_ = value;
    return RC::SUCCESS;
  }
  
  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s", 
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));
  
  Value::add(value, value_, value_);
  return RC::SUCCESS;
}

RC SumAggregator::evaluate(Value &result)
{
  result = value_;
  return RC::SUCCESS;
}
RC MaxAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED || value_.attr_type() == AttrType::NULL_) {
    value_ = value;
    return RC::SUCCESS;
  }

  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s",
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));

  if(this->value_.compare(value)) {
    this->value_ = value;
  }
  return RC::SUCCESS;
}
RC MaxAggregator::evaluate(Value &result)
{
  result = value_;
  return RC::SUCCESS;
}
RC MinAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED || value_.attr_type() == AttrType::NULL_) {
    value_ = value;
    return RC::SUCCESS;
  }

  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s",
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));

  if(this->value_.compare(value)  > 0) {
    this->value_ = value;
  }
  return RC::SUCCESS;
}
RC MinAggregator::evaluate(Value &result)
{
  result = value_;
  return RC::SUCCESS;
}
RC CountAggregator::accumulate(const Value &value)
{
  if (value_.attr_type() == AttrType::UNDEFINED || value_.attr_type() == AttrType::NULL_) {
    value_ = value;
    return RC::SUCCESS;
  }

  ASSERT(value.attr_type() == value_.attr_type(), "type mismatch. value type: %s, value_.type: %s",
        attr_type_to_string(value.attr_type()), attr_type_to_string(value_.attr_type()));


  countNum++;


  return RC::SUCCESS;
}
RC CountAggregator::evaluate(Value &result)
{
  result = Value(countNum);  // 假设Value支持从整数构造
  return RC::SUCCESS;
}
RC AvgAggregator::accumulate(const Value &value)
{
  // 忽略 UNDEFINED 和 NULL 值
  if (value.attr_type() != AttrType::UNDEFINED && value.attr_type() != AttrType::NULL_) {
    Value temp;
    // 累加值
    if (Value::add(sum_, value, temp) == RC::SUCCESS) {
      sum_ = temp;  // 更新总和
      countNum++;  // 计数
    }
  }
  return RC::SUCCESS;
}
RC AvgAggregator::evaluate(Value &result)
{
  if (countNum > 0) {
    Value temp;
    // 计算平均值
    if (Value::divide(sum_, Value(countNum), temp) == RC::SUCCESS) {
      result = temp;  // 返回平均值
    } else {
      // 处理除零的情况，返回 UNDEFINED 或 NULL
      result = Value();  // 或者根据需求设置为其他值
    }
  } else {
    // 处理没有有效值的情况，返回 UNDEFINED 或 NULL
    result = Value();  // 或者根据需求设置为其他值
  }
  return RC::SUCCESS;
}
