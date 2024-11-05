//
// Created by glwuy on 24-11-1.
//
#include "storage/index/ivfflat_index.h"

#include <queue>
#include <random>
#include <sql/expr/tuple.h>
#include <storage/db/db.h>
#include <storage/table/table.h>
#include "../../hnswlib/hnswlib.h"


RC IvfflatIndex::create(Table *           table, const char *file_name, const IndexMeta &index_meta,
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
  RC                 rc  = create_internal(table->db()->log_handler(), bpm, table, file_name);
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




RC IvfflatIndex::create_internal(LogHandler &log_handler, BufferPoolManager &bpm, Table *table, const char *file_name
    )
{
  // 创建文件。
  RC rc = bpm.create_file(file_name);
  if (rc != RC::SUCCESS) {
    LOG_WARN("Failed to create file. file name=%s, rc=%d:%s", file_name, rc, strrc(rc));
    return rc;
  }
  LOG_INFO("Successfully create index file:%s", file_name);
  DiskBufferPool *bp = nullptr;
  rc                 = bpm.open_file(log_handler, file_name, bp);
  if (rc != RC::SUCCESS) {
    LOG_WARN("Failed to open file. file name=%s, rc=%d:%s", file_name, rc, strrc(rc));
    return rc;
  }

  /**
   * 不考虑bufferPoolManager了，直接在内存中存储。
   *
   */
  this->table_ = table;
  this->datas_ = new std::vector<pair<IvfflatIndexKey, IvfflatIndexValue>>;
  // 确定维度，随机初始化簇中心。
  const FieldMeta *field_meta = this->field_meta_.at(1);
  this->key_field_meta_       = field_meta;
  if (field_meta->type() != AttrType::VECTORS) {
    return RC::INVALID_ARGUMENT;
  }
  this->dim_ = field_meta->len() / sizeof(float);
  // 随机初始化list_个簇，将其插入datas_中，
  // initialize_clusters();
  // 这个后面构造rowtuple然后获取值。
  FunctionExpr::type_from_string(this->func_name_.c_str(), this->func_type_);

  // Initing index
  this->space_ = new hnswlib::L2Space(this->dim_);
  int M = 8;                 // Tightly connected with internal dimensionality of the data
  // strongly affects the memory consumption
  int ef_construction = 80;
  this->alg_hnsw_ = new hnswlib::HierarchicalNSW<float>(this->space_, this->max_elements_, M, ef_construction);

  // 初始化完成。

  if (OB_FAIL(rc)) {
    bpm.close_file(file_name);
    return rc;
  }
  bpm.close_file(file_name);
  LOG_INFO("Successfully create index file %s.", file_name);
  return rc;
}

void IvfflatIndex::initialize_clusters()
{
  // 创建一个随机数生成器
  std::random_device                    rd;
  std::mt19937                          gen(rd());
  std::uniform_real_distribution<float> dis(0.0, static_cast<float>(this->lists_)); // 随机范围 [0.0, 100.0]

  // 初始化 datas_
  datas_ = new std::vector<std::pair<IvfflatIndexKey, IvfflatIndexValue>>();

  // 随机初始化 list_ 个簇
  for (int i = 0; i < lists_; ++i) {
    IvfflatIndexKey key;
    key.key.resize(dim_);

    // 随机生成一个向量
    for (int j = 0; j < dim_; ++j) {
      key.key[j] = dis(gen); // 生成随机值
    }

    // 创建一个空的 IvfflatIndexValue
    IvfflatIndexValue value;
    value.value = {}; // 初始化为空数组

    // 将键值对插入到 datas_ 中
    datas_->emplace_back(key, value);
  }
}

RC IvfflatIndex::open(Table *             table, const char *file_name, const IndexMeta &index_meta,
    const std::vector<const FieldMeta *> &field_meta)
{
  // 将持久化的内容读取出来。
  return RC::SUCCESS;
}


vector<RID> IvfflatIndex::ann_search(const vector<float> &base_vector, size_t limit)
{
  if(!temp_data_.empty()) {
    for (auto &n : temp_data_) {
      alg_hnsw_->addPoint(n->v().data(), this->count_);
      this->nodes_.emplace(this->count_,n);
      this->count_++;
    }
    temp_data_.clear();
  }
  vector<RID> result;
  std::priority_queue<std::pair<float, hnswlib::labeltype>> priority_queue = this->alg_hnsw_->searchKnn(base_vector.data(), limit);
  while(!priority_queue.empty()) {
    auto value = priority_queue.top();
    priority_queue.pop();
    VectorNode * vector_node = this->nodes_[value.second];
    result.push_back(vector_node->rid());
  }
  return result;
}

RC IvfflatIndex::close() { return RC::SUCCESS; }

RC IvfflatIndex::insert_entry(const char *record, const RID *rid)
{
  RC rc = RC::SUCCESS;
  // 将数据 reinterpret_cast 为 float* 并计算 float 数量
  std::vector<float> vec(this->key_field_meta_->len() / sizeof(float));

  // 使用 float_data 和 float_count 初始化 vector<float>
  memcpy(vec.data(), record + this->key_field_meta_->offset(), this->key_field_meta_->len());

  // // 检查全部的data，是否有需要分裂的。
  // if (this->count_ > 1 && this->count_ % (2 * this->lists_) == 0) {
  //   check_data();
  // }
  //
  //
  //
  //
  // float                                     dist_   = numeric_limits<float>::max();
  // pair<IvfflatIndexKey, IvfflatIndexValue> *target_ = nullptr;
  //
  // // O(n)
  // for (auto &k : *datas_) {
  //   float dist = compute_distance(vec, k.first.key); // 假设有个计算距离的函数
  //   if (dist_ > dist) {
  //     dist_   = dist;
  //     target_ = &k;
  //   }
  // }
  //
  // // 找到了距离最近的那个簇，加入进去并且更新簇的权重。
  VectorNode *node = new VectorNode(vec, *rid);
  // std::vector<VectorNode *> &target_nodes = target_->second.value;
  // target_nodes.push_back(node);
  this->temp_data_.emplace_back(node);

  return rc;
};

void IvfflatIndex::check_data()
{

  int                                       max_size       = -1;
  int                                       index          = -1;
  int                                       i              = 0;
  pair<IvfflatIndexKey, IvfflatIndexValue> *largest_target = nullptr;

  // 使用优先队列来找最小的probes个簇
  auto compare = [](const pair<IvfflatIndexKey, IvfflatIndexValue> *a,
      const pair<IvfflatIndexKey, IvfflatIndexValue> *              b) {
    return a->second.value.size() < b->second.value.size(); // 最小堆，较小的size优先
  };

  std::priority_queue<pair<IvfflatIndexKey, IvfflatIndexValue> *, std::vector<pair<IvfflatIndexKey, IvfflatIndexValue>
    *>, decltype(compare)> smallest_targets(compare);

  // 遍历数据找到大小最大的target和最小的probes个target
  for (auto &k : *datas_) {
    int current_size = k.second.value.size(); // 获取当前簇的大小
    i++;
    // 找到最大目标
    if (current_size > max_size) {
      max_size       = current_size;
      index          = i;
      largest_target = &k; // 记录最大簇
    }

    // 使用最小堆维护probes个最小的簇
    if (smallest_targets.size() < this->probes_) {
      smallest_targets.push(&k); // 如果堆未满，直接加入
    } else if (current_size < smallest_targets.top()->second.value.size()) {
      smallest_targets.pop();    // 移除堆顶（最大的最小簇）
      smallest_targets.push(&k); // 加入当前的簇
    }
  }

  // 检查是否需要分配
  if (largest_target && largest_target->second.value.size() > 2 * this->lists_) {
    auto & largest_nodes = largest_target->second.value; // 获取最大簇的节点
    size_t total_nodes   = largest_nodes.size();         // 获取节点总数

    // 将最小簇从优先队列转移到vector中
    std::vector<pair<IvfflatIndexKey, IvfflatIndexValue> *> smallest_targets_vector;
    while (!smallest_targets.empty()) {
      smallest_targets_vector.push_back(smallest_targets.top());
      smallest_targets.pop();
    }

    size_t num_targets      = smallest_targets_vector.size();  // 获取最小簇的数量
    size_t nodes_per_target = total_nodes / (num_targets + 1); // 计算每个目标应接收的节点数

    size_t distributed = 0; // 记录已分配的节点数量

    // 遍历每个最小目标，并将节点分配给它们
    for (size_t i = 0; i < num_targets; ++i) {
      auto &target_nodes = smallest_targets_vector[i]->second.value; // 当前目标的节点

      // 分配固定数量的节点
      for (size_t j = 0; j < nodes_per_target; ++j) {
        if (distributed < total_nodes) {
          target_nodes.push_back(largest_nodes[distributed++]); // 将节点添加到当前目标
        }
      }

      // 处理余数，确保前几个目标能够接收额外的节点
    }

    // 将已经分配的节点从原来的数组中删除。
    largest_nodes.erase(largest_nodes.begin(), largest_nodes.begin() + distributed);

    // 刷新所有目标的质心
    for (size_t i = 0; i < num_targets; ++i) {
      refresh_center(*smallest_targets_vector[i]); // 更新每个目标的质心
    }

    // 刷新最大目标的质心
    refresh_center(*largest_target);

    vector<int> size(this->lists_);
    int         i = 0;
    for (auto &k : *datas_) {
      size[i] = k.second.value.size();
      i++;
    }
    LOG_INFO("refresh:%d", index);

  }
}

std::vector<std::pair<IvfflatIndexKey, IvfflatIndexValue> *> *IvfflatIndex::find(const vector<float> &v)
{

  // 最小堆，存储距离和对应的键值对
  auto comp = [](const std::pair<float, std::pair<IvfflatIndexKey, IvfflatIndexValue> *> &a,
      const std::pair<float, std::pair<IvfflatIndexKey, IvfflatIndexValue> *> &           b) {
    return a.first < b.first; // 构建最大堆。
  };

  std::priority_queue<std::pair<float, std::pair<IvfflatIndexKey, IvfflatIndexValue> *>,
    std::vector<std::pair<float, std::pair<IvfflatIndexKey, IvfflatIndexValue> *>>,
    decltype(comp)> max_heap(comp);

  // 遍历所有键，计算距离
  for (auto &k : *datas_) {
    if (k.second.value.empty()) {
      // 空集合不比较。
      continue;
    }
    float dist = compute_distance(v, k.first.key);
    max_heap.emplace(dist, &k);

    // 保持堆的大小不超过 probes
    if (max_heap.size() > probes_) {
      max_heap.pop(); // 弹出最大的元素
    }
  }

  // 存储最近的 probes 个键值对
  auto *closest_keys = new std::vector<std::pair<IvfflatIndexKey, IvfflatIndexValue> *>;

  // 从最小堆中提取结果
  while (!max_heap.empty()) {
    closest_keys->push_back(max_heap.top().second);
    max_heap.pop();
  }

  return closest_keys;
}

IndexScanner *IvfflatIndex::create_scanner(const char *left_key, int left_len, bool left_inclusive,
    const char *                                       right_key,
    int                                                right_len, bool right_inclusive)
{
  IvfflatIndexScanner *index_scanner = new IvfflatIndexScanner(this);
  RC rc = index_scanner->open(left_key, left_len, left_inclusive, right_key, right_len, right_inclusive);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open index scanner. rc=%d:%s", rc, strrc(rc));
    delete index_scanner;
    return nullptr;
  }
  return index_scanner;
}

float IvfflatIndex::compute_distance(const vector<float> &left, const vector<float> &right)
{
  Value value;
  switch (func_type_) {
    case FunctionExpr::Type::L2_DISTANCE: {
      FunctionExpr::L2_DISTANCE(left, right, value);
    }
    break;
    case FunctionExpr::Type::COSINE_DISTANCE: {
      FunctionExpr::COSINE_DISTANCE(left, right, value);
      if (value.get_boolean()) {
        value.set_value(Value(numeric_limits<float>::max()));
      }
    }
    break;
    case FunctionExpr::Type::INNER_PRODUCT: {
      FunctionExpr::INNER_PRODUCT(left, right, value);
    }
    break;
  }
  return value.get_float();
}

void IvfflatIndex::refresh_center(pair<IvfflatIndexKey, IvfflatIndexValue> &data)
{
  auto &vectors = data.second.value;
  // 重新计算质心
  vector new_center(dim_, 0.0f);
  for (const auto &vector_node : vectors) {
    const auto &vector = vector_node->v();
    for (size_t i = 0; i < dim_; ++i) {
      new_center[i] += vector[i];
    }
  }

  // 计算平均值
  for (size_t i = 0; i < dim_; ++i) {
    new_center[i] /= vectors.size();
  }

  data.first.key.swap(new_center);
}

RC IvfflatIndex::delete_entry(const char *record, const RID *rid) { return RC::SUCCESS; };

RC IvfflatIndex::sync() { return RC::SUCCESS; };


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
IvfflatIndexScanner::IvfflatIndexScanner(IvfflatIndex *index)
{
  this->index_ = index;
}

IvfflatIndexScanner::~IvfflatIndexScanner() noexcept
{
}

RC IvfflatIndexScanner::open(const char *left_key, int left_len, bool left_inclusive, const char *right_key,
    int                                  right_len,
    bool                                 right_inclusive)
{
  RC rc = RC::SUCCESS;
  // 拿到的是一个vector_value。
  Value v;
  v.set_type(AttrType::VECTORS);
  v.set_data(left_key, left_len);

  Value limit;
  limit.set_type(AttrType::INTS);
  limit.set_data(right_key, right_len);

  this->limit_                     = limit.get_int();
  const std::vector<float> &vector = v.get_vector();

  std::vector<RID> res = index_->ann_search(vector, this->limit_);
  this->rids_.swap(res);
  // std::vector<std::pair<IvfflatIndexKey, IvfflatIndexValue> *> *key_values = index_->find(vector);
  //
  // // 使用最小堆来获取最近的向量
  // auto comp = [this, &vector](const auto &a, const auto &b) {
  //   return index_->compute_distance(vector, a->v()) > index_->compute_distance(vector, b->v());
  // };
  // std::priority_queue<VectorNode *, std::vector<VectorNode *>, decltype(comp)> min_heap(comp);
  //
  // int count                = 0;
  // int weight_percentages[] = {10, 20, 40, 60, 80}; // 扫描比例数组
  //
  // // 遍历每个簇
  // for (size_t i = 0; i < key_values->size(); ++i) {
  //   auto &key_value_pair = key_values->at(i);
  //   auto &nodes          = key_value_pair->second.value;
  //   int   node_size      = nodes.size();
  //
  //   // 计算要扫描的向量数量
  //   int scan_limit = static_cast<int>(node_size * (weight_percentages[i % 5] / 100.0));
  //
  //   for (int j = 0; j < scan_limit; ++j) {
  //     auto &node = nodes[j];
  //     count++;
  //
  //     if (min_heap.size() < this->limit_) {
  //       // 如果堆未满，直接插入
  //       min_heap.push(node);
  //     } else if (comp(node, min_heap.top())) {
  //       // 否则替换堆顶元素
  //       min_heap.pop();
  //       min_heap.push(node);
  //     }
  //   }
  // }
  //
  // LOG_INFO("this query q %d", count);
  //
  // // 将堆中的元素添加到数据中并按距离排序
  // this->data_.reserve(min_heap.size());
  // while (!min_heap.empty()) {
  //   this->data_.push_back(min_heap.top());
  //   min_heap.pop();
  // }
  //
  // // 不再需要排序，因为堆已按最近顺序组织
  // delete key_values;
  return rc;
}

RC IvfflatIndexScanner::next_entry(RID *rid)
{
  if (this->pos == this->rids_.size()) {
    this->pos = -1;
    return RC::RECORD_EOF;
  }
  pos++;
  // VectorNode *data = this->data_.at(pos);
  *rid             = this->rids_[pos];
  return RC::SUCCESS;
}

RC IvfflatIndexScanner::destroy()
{
  this->data_.clear();
  return RC::SUCCESS;
}