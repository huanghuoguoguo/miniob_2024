//
// Created by lyxahut on 24-10-21.
//

#pragma once

#include "common/sys/rc.h"
#include "common/type/char_type.h"

class TextType : public CharType
{
public:
  TextType() { attr_type_ = AttrType::TEXTS; }

  virtual ~TextType() = default;

};