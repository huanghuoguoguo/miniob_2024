//
// Created by glwuy on 24-11-1.
//
#include "storage/index/ivfflat_index.h"

RC IvfflatIndex::create(Table* table, const char* file_name, const IndexMeta& index_meta,
                    const std::vector<const FieldMeta*>& field_meta){
  return RC::SUCCESS;
  }

RC IvfflatIndex::open(Table *table, const char *file_name, const IndexMeta &index_meta, const std::vector<const FieldMeta*>& field_meta){
  return RC::SUCCESS;
  }


vector<RID> IvfflatIndex::ann_search(const vector<float> &base_vector, size_t limit) { return vector<RID>(); }

RC IvfflatIndex::close() { return RC::UNIMPLEMENTED; }

RC IvfflatIndex::insert_entry(const char *record, const RID *rid)  { return RC::UNIMPLEMENTED; };
RC IvfflatIndex::delete_entry(const char *record, const RID *rid)  { return RC::UNIMPLEMENTED; };

RC IvfflatIndex::sync(){ return RC::UNIMPLEMENTED; };