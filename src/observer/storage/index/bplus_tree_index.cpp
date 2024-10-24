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
// Created by wangyunlai.wyl on 2021/5/19.
//

#include "storage/index/bplus_tree_index.h"
#include "common/log/log.h"
#include "storage/table/table.h"
#include "storage/db/db.h"

BplusTreeIndex::~BplusTreeIndex() noexcept { close(); }

RC BplusTreeIndex::create(Table *         table, const char *file_name, const IndexMeta &index_meta,
    const std::vector<const FieldMeta *> &field_meta)
{
  if (inited_) {
    LOG_WARN("Failed to create index due to the index has been created before. file_name:%s, index:%s",
        file_name,
        index_meta.name());
    return RC::RECORD_OPENNED;
  }

  Index::init(index_meta, field_meta);

  BufferPoolManager &bpm = table->db()->buffer_pool_manager();
  RC rc = index_handler_.create(table->db()->log_handler(), bpm, file_name, field_meta);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to create index_handler, file_name:%s, index:%s, rc:%s",
        file_name,
        index_meta.name(),
        strrc(rc));
    return rc;
  }

  inited_ = true;
  LOG_INFO(
      "Successfully create index, file_name:%s, index:%s",
      file_name,
      index_meta.name());
  return RC::SUCCESS;
}

RC BplusTreeIndex::open(Table *table, const char *file_name, const IndexMeta &index_meta,const std::vector<const FieldMeta*>& field_meta)
{
  if (inited_) {
    LOG_WARN("Failed to open index due to the index has been initedd before. file_name:%s, index:%s",
        file_name,
        index_meta.name());
    return RC::RECORD_OPENNED;
  }

  Index::init(index_meta, field_meta);

  BufferPoolManager &bpm = table->db()->buffer_pool_manager();
  RC rc = index_handler_.open(table->db()->log_handler(), bpm, file_name);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to open index_handler, file_name:%s, index:%s, rc:%s",
        file_name,
        index_meta.name(),
        strrc(rc));
    return rc;
  }

  inited_ = true;
  LOG_INFO(
      "Successfully open index, file_name:%s, index:%s", file_name, index_meta.name());
  return RC::SUCCESS;
}

RC BplusTreeIndex::close()
{
  if (inited_) {
    LOG_INFO("Begin to close index, index:%s, field:%s", index_meta_.name(), index_meta_.field());
    index_handler_.close();
    inited_ = false;
  }
  LOG_INFO("Successfully close index.");
  return RC::SUCCESS;
}

RC BplusTreeIndex::insert_entry(const char *record, const RID *rid)
{
  // 计算总长度
  int total_len = 0;
  for (auto &field_meta : field_meta_) {
    total_len += field_meta->len();;
  }
  std::bitset<32> nullBitset;
  std::bitset<32> fieldNullBitset;
  // 读取第一个字段作为 bitset
  int nullInfo;
  memcpy(&nullInfo, record, sizeof(int)); // 从 record 中读取 null 信息
  nullBitset = std::bitset<32>(nullInfo); // 用读取的值初始化 bitset
  // 创建一个新的char数组来存储这些字段数据
  char *entry_data  = new char[total_len];
  int   current_pos = 0;
  // get_entry
  // 还需要判断，为null的列不能作为索引。
  for (size_t i = 0; i < field_meta_.size(); ++i) {
    auto &field_meta = field_meta_[i];
    int   offset     = field_meta->offset();
    int   len        = field_meta->len();
    // 将从record中提取的字段数据拷贝到entry_data
    memcpy(entry_data + current_pos, record + offset, len);
    current_pos += len;
    // 检查是否为 null
    if (nullBitset[i] == 1) {
      // 如果第 i 位为 1，表示该字段为 null
      fieldNullBitset.set(i); // 在第二个 bitset 中标记为 null
    }
  }
  unsigned int nullInfo2 = static_cast<unsigned int>(fieldNullBitset.to_ulong());
  memcpy(entry_data, &nullInfo2, sizeof(nullInfo2)); // 拷贝到 entry_data 的前四个字节


  // 如果不是唯一索引，不需要检查唯一性。
  if (index_meta_.is_unique()) {
    list<RID> rids;
    index_handler_.get_entry(entry_data, total_len, rids);
    // 释放分配的内存

    if (!rids.empty()) {
      delete[] entry_data;
      return RC::ERR_UNIQUE_INDEX_VIOLATION;
    }
  }
  RC rc = index_handler_.insert_entry(entry_data, rid);
  if (OB_FAIL(rc)) {
    delete[] entry_data;
    LOG_WARN("Failed to insert entry, rc:%s", strrc(rc));
    return rc;
  }
  return rc;
}

RC BplusTreeIndex::delete_entry(const char *record, const RID *rid)
{
  return index_handler_.delete_entry(record, rid);
}

IndexScanner *BplusTreeIndex::create_scanner(
    const char *left_key, int left_len, bool left_inclusive, const char *right_key, int right_len, bool right_inclusive)
{
  BplusTreeIndexScanner *index_scanner = new BplusTreeIndexScanner(index_handler_);
  RC rc = index_scanner->open(left_key, left_len, left_inclusive, right_key, right_len, right_inclusive);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open index scanner. rc=%d:%s", rc, strrc(rc));
    delete index_scanner;
    return nullptr;
  }
  return index_scanner;
}

RC BplusTreeIndex::sync() { return index_handler_.sync(); }

////////////////////////////////////////////////////////////////////////////////
BplusTreeIndexScanner::BplusTreeIndexScanner(BplusTreeHandler &tree_handler) : tree_scanner_(tree_handler) {}

BplusTreeIndexScanner::~BplusTreeIndexScanner() noexcept { tree_scanner_.close(); }

RC BplusTreeIndexScanner::open(
    const char *left_key, int left_len, bool left_inclusive, const char *right_key, int right_len, bool right_inclusive)
{
  return tree_scanner_.open(left_key, left_len, left_inclusive, right_key, right_len, right_inclusive);
}

RC BplusTreeIndexScanner::next_entry(RID *rid) { return tree_scanner_.next_entry(*rid); }

RC BplusTreeIndexScanner::destroy()
{
  delete this;
  return RC::SUCCESS;
}
