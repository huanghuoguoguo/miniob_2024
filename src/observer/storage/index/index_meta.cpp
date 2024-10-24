/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai.wyl on 2021/5/18.
//

#include "storage/index/index_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "storage/field/field_meta.h"
#include "storage/table/table_meta.h"
#include "json/json.h"

const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString FIELD_UNIQUE("unique");
const static Json::StaticString FIELD_FIELD_NAME("field_name");
const static Json::StaticString FIELD_FIELD_NAMES("field_names");

RC IndexMeta::init(const char *name, const std::vector<const FieldMeta *> &field_meta, bool is_unique)
{
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }

  name_      = name;
  is_unique_ = is_unique;
  for (const FieldMeta *field : field_meta) {
    field_.emplace_back(field->name());
  }
  return RC::SUCCESS;
}

void IndexMeta::to_json(Json::Value &json_value) const
{
  json_value[FIELD_NAME]   = name_;
  json_value[FIELD_UNIQUE] = is_unique_;

  Json::Value fields_value;
  for (const std::string &field : field_) {
    Json::Value field_value;
    field_value[FIELD_FIELD_NAME] = field;
    fields_value.append(std::move(field_value));
  }
  json_value[FIELD_FIELD_NAMES] = std::move(fields_value);
}

RC IndexMeta::from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index)
{
  const Json::Value &name_value   = json_value[FIELD_NAME];
  const Json::Value &fields_value = json_value[FIELD_FIELD_NAMES];
  const Json::Value &unique       = json_value[FIELD_UNIQUE];
  if (!name_value.isString()) {
    LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
    return RC::INTERNAL;
  }

  if (!unique.isBool()) {
    LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
    return RC::INTERNAL;
  }

  if (!fields_value.isArray() || fields_value.size() <= 0) {
    LOG_ERROR("Invalid table meta. fields is not array, json value=%s", fields_value.toStyledString().c_str());
    return RC::INTERNAL;
  }
  int                            field_num = fields_value.size();
  std::vector<const FieldMeta *> fields_list;
  for (int i = 0; i < field_num; i++) {
    const Json::Value &field_value = fields_value[i];
    const Json::Value &field_name  = field_value[FIELD_FIELD_NAME];
    if (!field_name.isString()) {
      LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
      return RC::INTERNAL;
    }
    const FieldMeta *field = table.field(field_name.asCString());
    if (nullptr == field) {
      LOG_ERROR("Deserialize index [%s]: no such field: %s", name_value.asCString(), field_value.asCString());
      return RC::SCHEMA_FIELD_MISSING;
    }
    fields_list.push_back(field);
  }
  return index.init(name_value.asCString(), fields_list, unique.asBool());
}

const char *IndexMeta::name() const { return name_.c_str(); }

const std::vector<std::string> IndexMeta::field() const
{
  return field_;
}

void IndexMeta::desc(ostream &os) const
{
  os << "index name=" << name_ << ", field=";

  for (const std::string &field : field_) {
    os << field;
    os << ",";
  }
}