//
// Created by glwuy on 24-11-1.
//
#include "storage/index/ivfflat_index.h"

#include <random>
#include <sql/expr/tuple.h>
#include <storage/db/db.h>
#include <storage/table/table.h>

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
  initialize_clusters();
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


vector<RID> IvfflatIndex::ann_search(const vector<float> &base_vector, size_t limit) { return vector<RID>(); }

RC IvfflatIndex::close() { return RC::SUCCESS; }

RC IvfflatIndex::insert_entry(const char *record, const RID *rid)
{
  RC rc = RC::SUCCESS;
  // 取到定义的值
  Value v;
  v.set_type(AttrType::VECTORS);
  int offset = this->key_field_meta_->offset();
  int len    = this->key_field_meta_->len();
  v.set_data(record + offset, len);
  // 获取到值之后，循环所有键，得到list_个键值，然后在这些键值中循环找到距离最小的，插入后调整。
  vector<float> key = v.get_vector();
  if (key.size() != dim_) {
    return RC::INVALID_ARGUMENT;
  }

  float                                     dist_   = numeric_limits<float>::max();
  pair<IvfflatIndexKey, IvfflatIndexValue> *target_ = nullptr;
  for (auto &k : *datas_) {
    float dist = compute_distance(key, k.first.key); // 假设有个计算距离的函数
    if (dist_ > dist) {
      dist_   = dist;
      target_ = &k;
    }
  }
  ASSERT(target_ != nullptr, "The key does not exist.");
  // 找到了距离最近的那个簇，加入进去并且更新簇的权重。
  VectorNode *node = new VectorNode(key, *rid);
  target_->second.value.push_back(node);
  // 如果size > 100 刷新键的质心。
  if (target_->second.value.size() > this->lists_) {
    refresh_center(*target_);
  }
  return rc;
};

std::vector<std::pair<IvfflatIndexKey, IvfflatIndexValue> *> *IvfflatIndex::find(Value &v)
{
  // 获取到值之后，循环所有键，得到list_个键值，然后在这些键值中循环找到距离最小的，插入后调整。
  vector<float> key = v.get_vector();
  if (key.size() != dim_) {
    return nullptr;
  }
  // 存储距离与对应的键值对
  std::vector<std::pair<float, std::pair<IvfflatIndexKey, IvfflatIndexValue> *>> distances;

  // 遍历所有键，计算距离
  for (auto &k : *datas_) {
    float dist = compute_distance(key, k.first.key); // 假设有个计算距离的函数
    distances.emplace_back(dist, &k);
  }

  // 按距离排序
  std::sort(distances.begin(),
      distances.end(),
      [](const auto &a, const auto &b) { return a.first < b.first; });

  // 获取最近的 probes 个键值对
  auto *closest_keys = new std::vector<std::pair<IvfflatIndexKey, IvfflatIndexValue> *>;
  for (size_t i = 0; i < std::min(static_cast<size_t>(probes_), distances.size()); ++i) {
    closest_keys->push_back(distances[i].second);
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

float IvfflatIndex::compute_distance(vector<float> left, vector<float> right)
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
  vector<float> new_center(dim_, 0.0f);
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

  data.first.key = new_center;
}

RC IvfflatIndex::delete_entry(const char *record, const RID *rid) { return RC::SUCCESS; };

RC IvfflatIndex::sync() { return RC::SUCCESS; };


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
IvfflatIndexScanner::IvfflatIndexScanner(IvfflatIndex* index)
{
  this->index_ = index;
}
IvfflatIndexScanner::~IvfflatIndexScanner() noexcept {   }
RC IvfflatIndexScanner::open(const char *left_key, int left_len, bool left_inclusive, const char *right_key,
    int                                  right_len,
    bool                                 right_inclusive)
{
  RC rc = RC::SUCCESS;
  // 拿到的是一个vector_value。
  Value v;
  v.set_type(AttrType::VECTORS);
  v.set_data(left_key, left_len);
  std::vector<std::pair<IvfflatIndexKey, IvfflatIndexValue> *> *key_values = index_->find(v);

  if (key_values == nullptr) {
    delete key_values;
    return RC::SUCCESS;
  }
  // 获取所有的值。
  for (int i = 0; i < key_values->size(); ++i) {
    auto &nodes = key_values->at(i)->second.value;
    for (auto &k : nodes) {
      this->data_.push_back(k);
    }
  }
  // 然后将值排序。
  std::sort(this->data_.begin(),
      this->data_.end(),
      [this](const auto &a, const auto &b) { return index_->compute_distance(a->v(), b->v()) < 0; });
  // 根据v获取到值集合，然后记录位置。
  return rc;
}

RC IvfflatIndexScanner::next_entry(RID *rid)
{
  if (this->pos == this->data_.size() - 1) {
    return RC::RECORD_EOF;
  }
  pos++;
  VectorNode *data = this->data_.at(pos);
  *rid             = data->rid();
  return RC::SUCCESS;
}

RC IvfflatIndexScanner::destroy()
{
  this->data_.clear();
  return RC::SUCCESS;
}