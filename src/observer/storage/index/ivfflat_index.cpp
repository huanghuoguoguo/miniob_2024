//
// Created by glwuy on 24-11-1.
//
#include "storage/index/ivfflat_index.h"

#include <queue>
#include <random>
#include <event/sql_debug.h>
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



  // 初始化完成。

  if (OB_FAIL(rc)) {
    bpm.close_file(file_name);
    return rc;
  }
  bpm.close_file(file_name);
  LOG_INFO("Successfully create index file %s.", file_name);
  return rc;
}



RC IvfflatIndex::open(Table *             table, const char *file_name, const IndexMeta &index_meta,
    const std::vector<const FieldMeta *> &field_meta)
{
  // 将持久化的内容读取出来。
  return RC::SUCCESS;
}

void IvfflatIndex::init_data()
{
  sql_debug("start init index data");
  // Initing index
  this->space_    = new hnswlib::L2Space(this->dim_);
  this->key_hnsw_ = new hnswlib::HierarchicalNSW<float>(this->space_, this->lists_, 16, 160);
  Matrix data(temp_data_.size(), nullptr);
  for (int i = 0; i < temp_data_.size(); ++i) {
    std::vector<float> &vector = temp_data_[i]->v();
    data[i]                    = &vector;
  }
  this->nodes_.resize(temp_data_.size());
  // 聚类中心和标签
  Matrix centers(this->lists_);
  for (auto i = 0; i < this->lists_; ++i) {
    centers[i] = new Vector(this->dim_);
  }
  std::vector<int> labels(temp_data_.size(), -1);
  std::vector<int>             count = kmeans(data, centers, labels);

  sql_debug("kmeans finished");
  // 经过聚类之后，得到了245个中心位置，以及60000个向量的标签。接下来就是构建245个索引，然后再构建1个索引用于索引245个索引。
  this->hnsw_node_.resize(this->lists_);
  // 将所有向量按照标签放入桶中。
  for (int i = 0; i < this->temp_data_.size(); ++i) {
    int         label            = labels[i];
    VectorNode *node             = temp_data_.at(i);
    hnswlib::HierarchicalNSW<float> * &hierarchical_nsw = this->hnsw_node_[label];
    if(hierarchical_nsw == nullptr) {
      this->hnsw_node_[label] = new hnswlib::HierarchicalNSW<float>(this->space_,
        count[label],
        this->M,
        this->ef_construction);
      hierarchical_nsw = this->hnsw_node_[label];
    }
    hierarchical_nsw->addPoint(node->v().data(), i);
    this->nodes_[i] = node->rid();
    delete node;
  }
  // 构建key的桶。标签对应的是hnsw_node_中的桶。
  for (int i = 0; i < this->lists_; i++) {
    this->key_hnsw_->addPoint(centers[i]->data(), i);
  }
  sql_debug("end init index data");
  temp_data_.clear();
}

vector<RID> IvfflatIndex::ann_search(const vector<float> &base_vector, size_t limit)
{
  if (!temp_data_.empty()) {
    init_data();
  }
  vector<RID> result;
  // 拿到最近的probes个桶放入keys后，从每个桶中获取到limit个条目。
  // 维护大小为limit的优先队列，存储<float,int> float是距离，int是到node_中查找rid的索引。
  auto compare = [](const DistanceRIDPair& a, const DistanceRIDPair& b) {
    return a.first < b.first; // 最大堆，距离小的优先
  };
  std::priority_queue<DistanceRIDPair, std::vector<DistanceRIDPair>, decltype(compare)> max_heap(compare);

  // 找到最近的probes个桶。
  std::priority_queue<std::pair<float, hnswlib::labeltype>> key_queue = this->key_hnsw_->searchKnn(base_vector.data(),
      this->probes_);
  while (!key_queue.empty()) {
    auto value = key_queue.top();
    key_queue.pop();
    hnswlib::HierarchicalNSW<float> *                         need_search_node = this->hnsw_node_[value.second];
    if(need_search_node == nullptr) {
      continue;
    }
    std::priority_queue<std::pair<float, hnswlib::labeltype>> priority_queue   = need_search_node->searchKnn(
        base_vector.data(),
        limit);
    while (!priority_queue.empty()) {
      auto node_value = priority_queue.top();
      priority_queue.pop();
      const unsigned long &rid_index = node_value.second;

      // 计算当前距离
      const float distance = node_value.first;

      // 如果堆的大小小于limit，直接插入
      if (max_heap.size() < limit) {
        max_heap.emplace(distance, rid_index);
      } else if (distance < max_heap.top().first) { // 如果当前距离更小，则替换堆顶
        max_heap.pop();
        max_heap.emplace(distance, rid_index);
      }
    }
  }


  // 将结果从最大堆中提取出来
  while (!max_heap.empty()) {
    int second = max_heap.top().second;
    result.push_back(this->nodes_[second]); // 获取RID
    max_heap.pop();
  }

  // 结果可能是反向的，按距离排序
  std::ranges::reverse(result);
  // 维护优先队列，
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
  VectorNode *node = new VectorNode(vec, *rid);
  this->temp_data_.push_back(node);
  if (temp_data_.size() == 60000) {
    init_data();
    temp_data_.clear();
  }
  return rc;
};





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
      float sum = 0.0f;
      for (size_t i = 0; i < left.size(); ++i) {
        float diff = left[i] - right[i];
        sum += diff * diff;
      }
      return sum;
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

std::vector<int> IvfflatIndex::kmeans(const Matrix &data, Matrix &centers, std::vector<int> &labels)
{
  const int NUM_CLUSTERS = this->lists_;

  int              num_samples    = data.size();
  int              num_dimensions = data[0]->size();
  std::vector<int> counts(NUM_CLUSTERS, 0);
  bool             changed;

  // 初始化聚类中心
  for (int i = 0; i < NUM_CLUSTERS; ++i) {
    centers[i] = data[std::rand() % num_samples];
  }

  for (int iter = 0; iter < 2; ++iter) {
    changed = false;

    // 分配标签
    for (int i = 0; i < num_samples; ++i) {
      int   closest_center = -1;
      float min_distance   = std::numeric_limits<float>::max();

      for (int j = 0; j < NUM_CLUSTERS; ++j) {
        float distance = compute_distance(*data[i], *centers[j]);
        if (distance < min_distance) {
          min_distance   = distance;
          closest_center = j;
        }
      }

      if (labels[i] != closest_center) {
        labels[i] = closest_center;
        changed   = true;
      }
    }

    // 更新聚类中心
    for (int i = 0; i < NUM_CLUSTERS; ++i) {
      centers.at(i) = new std::vector<float>(num_dimensions, 0.0f);
    }
    counts.assign(NUM_CLUSTERS, 0);

    for (int i = 0; i < num_samples; ++i) {
      int cluster = labels[i];
      counts[cluster]++;
      for (int d = 0; d < num_dimensions; ++d) {
        centers[cluster]->at(d) += data[i]->at(d);
      }
    }

    for (int j = 0; j < NUM_CLUSTERS; ++j) {
      if (counts[j] > 0) {
        for (int d = 0; d < num_dimensions; ++d) {
          centers[j]->at(d) /= counts[j];
        }
      }
    }

    // 如果没有改变，停止迭代
    if (!changed) {
      break;
    }
  }
  return counts;
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
  return rc;
}

RC IvfflatIndexScanner::next_entry(RID *rid)
{
  if (this->pos == this->rids_.size() - 1) {
    return RC::RECORD_EOF;
  }
  pos++;
  // VectorNode *data = this->data_.at(pos);
  *rid = this->rids_[pos];
  return RC::SUCCESS;
}

RC IvfflatIndexScanner::destroy()
{
  this->data_.clear();
  return RC::SUCCESS;
}