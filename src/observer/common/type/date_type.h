//
// Created by bob on 24-9-16.
//

#pragma once

#include <common/log/log.h>

#include "common/rc.h"
#include "common/type/data_type.h"

/**
 * @brief 日期类型
 * @ingroup DataType
 */
class DateType : public DataType {
public:
    DateType() : DataType(AttrType::DATES) {
    }

    virtual ~DateType() = default;

    static bool check_dateV2(int year, int month, int day) {
        static int mon[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        LOG_WARN("check_dateV2: year %d,month %d,day %d");
        bool leap = (year % 400 == 0 || (year % 100 && year % 4 == 0));
        if (year > 0 && (month > 0) && (month <= 12) && (day > 0) && (
                day <= ((month == 2 && leap) ? 1 : 0) + mon[month]))
            return true;
        else
            return false;
    }

    static int string_to_date(const std::string &str, int32_t &date) {
        int y, m, d;
        sscanf(str.c_str(), "%d-%d-%d", &y, &m, &d); //not check return value eq 3, lex guarantee
        bool b = check_dateV2(y, m, d);
        if (!b) return -1;
        date = y * 10000 + m * 100 + d;
        return 0;
    }

    static std::string date_to_string(int32_t date) {
        std::string ans = "YYYY-MM-DD";
        std::string tmp = std::to_string(date);
        int tmp_index = 7;
        for (int i = 9; i >= 0; i--) {
            if (i == 7 || i == 4) {
                ans[i] = '-';
            } else {
                ans[i] = tmp[tmp_index--];
            }
        }
        return ans;
    }

    int compare(const Value &left, const Value &right) const override;

    RC set_value_from_str(Value &val, const string &data) const override;

    RC to_string(const Value &val, string &result) const override;
};
