//
// Created by bob on 24-9-16.
//
#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/date_type.h"
#include "common/value.h"

RC DateType::to_string(const Value &val, string &result) const
{
  std::stringstream ss;

  int32_t int_value = val.value_.int_value_;
  std::string       ans       = "YYYY-MM-DD";
  std::string       tmp       = std::to_string(int_value);
  int               tmp_index = 7;
  for (int i = 9; i >= 0; i--) {
    if (i == 7 || i == 4) {
      ans[i] = '-';
    } else {
      ans[i] = tmp[tmp_index--];
    }
  }
  ss << ans;

  result = ss.str();
  return RC::SUCCESS;
}

int DateType::compare(const Value &left, const Value &right) const
{   if (left.attr_type() == AttrType::DATES && right.attr_type() == AttrType::DATES)
    return common::compare_int((void *)&left.value_.int_value_, (void *)&right.value_.int_value_);

    return INT32_MAX;
}

RC DateType::set_value_from_str(Value &val, const string &data) const
{
  RC                rc = RC::SUCCESS;
  stringstream deserialize_stream;
  deserialize_stream.clear();  // 清理stream的状态，防止多次解析出现异常
  deserialize_stream.str(data);
  int int_value;
  deserialize_stream >> int_value;
  if (!deserialize_stream || !deserialize_stream.eof()) {
    rc = RC::SCHEMA_FIELD_TYPE_MISMATCH;
  } else {
    val.set_int(int_value);
  }
  return rc;
}

bool check_dateV2(int year, int month, int day)
{
  static int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  LOG_WARN("check_dateV2: year %d,month %d,day %d");
  bool leap = (year % 400 == 0 || (year % 100 && year % 4 == 0));
  if (year > 0 && (month > 0) && (month <= 12) && (day > 0) && (day <= ((month == 2 && leap) ? 1 : 0) + mon[month]))
    return true;
  else
    return false;
}

int string_to_date(const std::string &str,int32_t & date)
{
  int y,m,d;
  sscanf(str.c_str(), "%d-%d-%d", &y, &m, &d);//not check return value eq 3, lex guarantee
  bool b = check_dateV2(y,m,d);
  if(!b) return -1;
  date = y*10000+m*100+d;
  return 0;
}

std::string date_to_string(int32_t date)
{
  std::string ans = "YYYY-MM-DD";
  std::string tmp = std::to_string(date);
  int tmp_index = 7;
  for(int i = 9 ; i >=0 ;i--)
  {
    if(i == 7|| i == 4)
    {
      ans[i] = '-';
    }
    else
    {
      ans[i] = tmp[tmp_index--];
    }
  }
  return ans;
}