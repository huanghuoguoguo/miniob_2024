//
// Created by glwuy on 24-10-21.
//

#include "list_type.h"
#include "common/log/log.h"
#include "common/value.h"

int ListType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::UNDEFINED, "left type is not NULL");
  if (right.attr_type() == AttrType::UNDEFINED)
    return 0;
  return INT32_MAX;
}
bool ListType::count(Value* value)
{
  for (auto it = values.begin(); it != values.end(); ++it) {
    if ((*it)->compare(*value) == 0) {
      return true;
    }
  }
  return false;
}