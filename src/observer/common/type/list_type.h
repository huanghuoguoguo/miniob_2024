//
// Created by glwuy on 24-10-21.
//

#ifndef LIST_TYPE_H
#define LIST_TYPE_H
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
    void add(Value* value) { values.emplace_back(value); }
    bool empty() { return this->values.empty(); }
    void get_value(Value& value)
    {
        if(values.size()>1) {
            value.set_list(&values);
        }else {
            value.set_value(*values.at(0));
        }
    }
private:
    std::vector<Value*> values;
};


#endif //LIST_TYPE_H
