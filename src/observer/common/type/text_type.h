//
// Created by lyxahut on 24-10-21.
//

#pragma once

#include "common/rc.h"
#include "common/type/data_type.h"
#include "char_type.h"
#include <string>

/**
 * @brief 可变长度的字符串类型(TEXT)
 * @ingroup DataType
 */
class TextType : public CharType
{
public:
  TextType() : CharType(AttrType::TEXTS) {}

  virtual ~TextType() = default;

};

