/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include <regex>
#include "common/lang/comparator.h"
#include "common/log/log.h"
#include "common/type/char_type.h"
#include "common/value.h"

int CharType::compare(const Value &left, const Value &right) const
{
  if (right.is_null())
    return INT32_MAX;
  ASSERT(left.attr_type() == AttrType::CHARS && right.attr_type() == AttrType::CHARS, "invalid type");
  return common::compare_string(
      (void *)left.value_.pointer_value_, left.length_, (void *)right.value_.pointer_value_, right.length_);
}

RC CharType::set_value_from_str(Value &val, const string &data) const
{
  val.set_string(data.c_str());
  return RC::SUCCESS;
}

RC CharType::cast_to(const Value &val, AttrType type, Value &result) const
{

  switch (type) {
    case AttrType::INTS:
    case AttrType::FLOATS: {
      auto extract_number = [](const std::string &str) -> std::string {
        std::regex  re("^([0-9]+\\.?[0-9]*)");
        std::smatch match;
        if (std::regex_search(str, match, re)) {
          return match.str(0); // 返回匹配到的第一个数字部分
        }
        return "error"; // 如果没有找到数字部分，返回 0
      };
      auto str = extract_number(val.get_string());
      if (str == "error") {
        str = "0";
      }
      if (type == AttrType::INTS) {
        result.set_int(std::stoi(str));
      } else if (type == AttrType::FLOATS) {
        result.set_float(std::stof(str));
      }
      break;
    }
    case AttrType::VECTORS: {
      std::vector<float> numbers;

      auto extract_vector = [&](const char* data, int length) -> void {
        const char* end = data + length;
        const char* current = data + 1; // 跳过开头的 '['

        while (current < end && *current != ']') {
          char* next;
          // 使用 strtof 解析浮点数
          float num = std::strtof(current, &next);

          if (current == next) {
            // 如果未解析成功，说明遇到非法字符
            break;
          }

          numbers.push_back(num);

          // 更新 current 指针位置，跳过 ',' 或结束
          current = (*next == ',') ? next + 1 : next;
        }
      };

      extract_vector(val.data(), val.length());

      if (numbers.empty()) {
        return RC::EMPTY;
      }

      result.set_vector(numbers);
      break;
    }
    default: return RC::UNIMPLEMENTED;
  }
  return RC::SUCCESS;
}

int CharType::cast_cost(AttrType type)
{
  if (type == AttrType::CHARS) {
    return 0;
  } else if (type == AttrType::INTS || type == AttrType::FLOATS) {
    return 1;
  } else if (type == AttrType::VECTORS) {
    return 2;
  }
  return INT32_MAX;
}

RC CharType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << val.value_.pointer_value_;
  result = ss.str();
  return RC::SUCCESS;
}