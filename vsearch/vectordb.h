#ifndef VECTORSEARCH_VSEARCH_VECTORDB_H_
#define VECTORSEARCH_VSEARCH_VECTORDB_H_

#include <string>

#include "rocksdb/db.h"
#include "rocksdb/options.h"

class VectorDB {
 private:
  const std::string kDBPath = "/tmp/vector_server_index";

  rocksdb::DB* db;
  rocksdb::Options options_;

 public:
  VectorDB();
  ~VectorDB();

  rocksdb::Status openDB();
  rocksdb::Status put(std::string key, std::string value);
  rocksdb::Status get(std::string key, std::string *value);
};

#endif //VECTORSEARCH_VSEARCH_VECTORDB_H_
