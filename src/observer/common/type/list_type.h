//
// Created by glwuy on 24-10-21.
//

#pragma once
#include <common/value.h>

#include "data_type.h"


class ListType : public DataType
{
public:
    ListType() : DataType(AttrType::UNDEFINED)
    {
    }

    virtual ~ListType() = default;
    int compare(const Value& left, const Value& right) const override;
    bool count(Value* value);
    void add(Value* value) { values.emplace_back(value); }
    bool empty() { return this->values.empty(); }
    int size() { return this->values.size(); }

    void get_value(Value& value)
    {
        if (values.size() > 1)
        {
            value.set_list(&values);
        }
        else
        {
            value.set_value(*values.at(0));
        }
    }

    std::vector<Value*>& values_vector()
    {
        return values;
    }

    void values_vector(const std::vector<Value*>& values)
    {
        this->values = values;
    }

private:
    std::vector<Value*> values;
};



