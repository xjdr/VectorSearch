#include <cstdio>
#include <cassert>

#include "VectorDB.h"

VectorDB::VectorDB() {
  assert(openDB().ok());
}

rocksdb::Status VectorDB::openDB() {
  options_.create_if_missing = true;
  rocksdb::Status s = rocksdb::DB::Open(options_, kDBPath, &db);
  assert(s.ok());

  return s;
}

rocksdb::Status VectorDB::put(std::string key, std::string value) {
  const rocksdb::Status s = db->Put(rocksdb::WriteOptions(), key, value);
  assert(s.ok());

  return s;
}

rocksdb::Status VectorDB::get(std::string key, std::string *value) {
   const rocksdb::Status s = db->Get(rocksdb::ReadOptions(), key, value);
   assert(s.ok());

  return s;
}

VectorDB::~VectorDB() {
  // close db
  delete db;
}
