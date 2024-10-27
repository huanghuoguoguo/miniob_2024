#include "vector_type.h" // 假设这个头文件中声明了 VectorType 类

#include <iomanip>

#include "common/value.h"
#include <vector>
#include <sstream>
#include <string>



int VectorType::compare(const Value &left, const Value &right) const 
{ 
    if (left.is_null() || right.is_null()) 
        return INT32_MAX;

    return 0; // 相等
} 

RC VectorType::add(const Value &left, const Value &right, Value &result) const 
{ 

    return RC::SUCCESS; 
} 

RC VectorType::subtract(const Value &left, const Value &right, Value &result) const 
{ 

    return RC::SUCCESS; 
} 

RC VectorType::multiply(const Value &left, const Value &right, Value &result) const 
{ 

    return RC::SUCCESS; 
}

RC VectorType::to_string(const Value &val, std::string &result) const
{
  std::vector<float> vec = val.get_vector();
  string oss;
  oss += "["; // 添加开头的方括号

  for (size_t i = 0; i < vec.size(); ++i) {
    oss += formatFloat(vec[i]);
    if (i < vec.size() - 1) {
      oss += ","; // 添加逗号，除了最后一个元素
    }
  }

  oss += "]"; // 添加结尾的方括号
  result = oss.c_str();
  return RC::SUCCESS;
}

RC VectorType::cast_to(const Value &val, AttrType type, Value &result) const 
{ 
	switch (type){
      case AttrType::CHARS:{
        std::vector<float> vec = val.get_vector();
        string oss;
        oss += "["; // 添加开头的方括号

        for (size_t i = 0; i < vec.size(); ++i) {
          oss += formatFloat(vec[i]);
          if (i < vec.size() - 1) {
            oss += ","; // 添加逗号，除了最后一个元素
          }
        }

        oss += "]"; // 添加结尾的方括号
        result.set_string(oss.c_str());
      }break;
    default: return RC::UNIMPLEMENTED;
      }
    return RC::SUCCESS;
}

int VectorType::cast_cost(AttrType type)
{
  if (type == AttrType::INTS) {
    return 0;
  } else if (type == AttrType::CHARS || type == AttrType::FLOATS) {
    return -1;
  }
  return INT32_MAX;
}
string VectorType::formatFloat(float value) const
{
  char buffer[32]; // 足够大的缓冲区
  // 使用 snprintf 格式化浮点数
  int len = snprintf(buffer, sizeof(buffer), "%.2f", value);

  // 将结果转换为 std::string
  std::string result(buffer, len);

  // 去掉末尾的多余零
  if (result.find('.') != std::string::npos) {
    result.erase(result.find_last_not_of('0') + 1, std::string::npos); // 去掉末尾的0
    if (result.back() == '.') {
      result.pop_back(); // 去掉小数点
    }
  }

  return result;
}