//
// Created by glwuy on 24-9-17.
//

#ifndef NULL_TYPE_H
#define NULL_TYPE_H
#include "data_type.h"


class NullType : public DataType
{
public:
  NullType() : DataType(AttrType::NULL_) {}
  virtual ~NullType() = default;

  int compare(const Value &left, const Value &right) const override;

};


#endif //NULL_TYPE_H