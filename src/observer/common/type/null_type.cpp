//
// Created by glwuy on 24-9-17.
//

#include "null_type.h"
#include "common/lang/comparator.h"
#include "common/lang/sstream.h"
#include "common/log/log.h"
#include "common/type/integer_type.h"
#include "common/value.h"

int NullType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::UNDEFINED, "left type is not NULL");
  if (right.attr_type() == AttrType::UNDEFINED)
    return 0;
  return INT32_MAX;
}