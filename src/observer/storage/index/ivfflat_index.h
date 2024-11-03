/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include <storage/db/db.h>

#include <utility>
#include <unordered_map>
#include <sql/expr/expression.h>

#include "storage/index/index.h"

struct IvfflatIndexValue;
class VectorNode;
struct IvfflatIndexKey;
/**
 * @brief ivfflat 向量索引
 * @ingroup Index
 */
class IvfflatIndex : public Index
{
public:
    IvfflatIndex(int lists, int probes,string func_name)
    {
        this->lists_ = lists;
        this->probes_ = probes;
        func_name_ = func_name;
    };

    virtual ~IvfflatIndex() noexcept
    {
    };


    RC create(Table* table, const char* file_name, const IndexMeta& index_meta,
              const std::vector<const FieldMeta*>& field_meta) override;

    RC open(Table* table, const char* file_name, const IndexMeta& index_meta,
            const std::vector<const FieldMeta*>& field_meta) override;

    bool is_vector_index() override { return true; }
    vector<RID> ann_search(const vector<float>& base_vector, size_t limit);

    RC close();

    RC insert_entry(const char* record, const RID* rid) override;
    RC delete_entry(const char* record, const RID* rid) override;
    std::vector<std::pair<IvfflatIndexKey, IvfflatIndexValue>*>* find(Value& v);

    RC sync() override;

    IndexScanner* create_scanner(const char* left_key, int left_len, bool left_inclusive, const char* right_key,
                                 int right_len, bool right_inclusive) override;

    void lists(int lists)
    {
        lists_ = lists;
    }
    void probes(int probes)
    {
        probes_ = probes;
    };
    float compute_distance(vector<float> left, vector<float> right);
private:
    RC create_internal(LogHandler& log_handler, BufferPoolManager& bpm, Table* table, const char* file_name);
    void initialize_clusters();

    void refresh_center(pair<IvfflatIndexKey, IvfflatIndexValue>& data);
private:
    bool inited_ = false;
    Table* table_ = nullptr;
    int lists_ = 1;
    int probes_ = 1;
    // index_meta在父类已经有了。
    std::vector<pair<IvfflatIndexKey, IvfflatIndexValue>>* datas_ = nullptr;
    unsigned int dim_;
    // 还需要确定维度。
    const FieldMeta* key_field_meta_ = nullptr;
    string func_name_;
    FunctionExpr::Type func_type_;
};
class IvfflatIndexScanner : public IndexScanner
{
public:
    explicit IvfflatIndexScanner(IvfflatIndex* index);
    ~IvfflatIndexScanner() noexcept override;

    RC next_entry(RID *rid) override;
    RC destroy() override;

    // open的时候找到所有的值吧应该。
    RC open(const char *left_key, int left_len, bool left_inclusive, const char *right_key, int right_len,
        bool right_inclusive);

private:
    // 记录扫描位置。
    IvfflatIndex* index_ = nullptr;
    int pos = -1;
    std::vector<VectorNode*> data_;
};

struct  IvfflatIndexKey
{
public:
    vector<float> key;
};

struct IvfflatIndexValue
{
    vector<VectorNode*> value;
};
/**
 * 这个类被用于保存键和rid
 */
class VectorNode
{
    public:
    VectorNode(std::vector<float> v,RID rid)
    {
        v_ = std::move(v);
        rid_ = rid;
    };

    virtual ~VectorNode() noexcept
    {
    };

    std::vector<float> v() const
    {
      return v_;
    }

    void v(const std::vector<float> &v)
    {
      v_ = v;
    }

    RID& rid()
    {
      return rid_;
    }

    void rid(const RID &rid)
    {
      rid_ = rid;
    }

  private:
    std::vector<float> v_;
    RID rid_;
};