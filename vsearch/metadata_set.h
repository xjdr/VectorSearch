// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef VECTORSEARCH_VSEARCH_METADATA_SET_H_
#define VECTORSEARCH_VSEARCH_METADATA_SET_H_

#include "common_data_structure.h"

#include <fstream>
#include <iostream>

namespace vsearch {

class MetadataSet {
 public:
  MetadataSet();

  virtual ~MetadataSet();

  virtual ByteArray GetMetadata(IndexType p_vectorID) const = 0;

  virtual SizeType Count() const = 0;

  virtual bool Available() const = 0;

  virtual void AddBatch(MetadataSet& data) = 0;

  virtual ErrorCode SaveMetadata(const std::string& p_metaFile,
                                 const std::string& p_metaindexFile) = 0;

  virtual ErrorCode RefineMetadata(std::vector<int>& indices,
                                   const std::string& p_folderPath);

  static ErrorCode MetaCopy(const std::string& p_src, const std::string& p_dst);
};

class FileMetadataSet : public MetadataSet {
 public:
  FileMetadataSet(const std::string& p_metaFile,
                  const std::string& p_metaindexFile);

  ~FileMetadataSet();

  ByteArray GetMetadata(IndexType p_vectorID) const;

  SizeType Count() const;

  bool Available() const;

  void AddBatch(MetadataSet& data);

  ErrorCode SaveMetadata(const std::string& p_metaFile,
                         const std::string& p_metaindexFile);

 private:
  std::ifstream* m_fp = nullptr;

  std::vector<std::uint64_t> m_pOffsets;

  SizeType m_count;

  std::string m_metaFile;

  std::string m_metaindexFile;

  std::vector<std::uint8_t> m_newdata;
};

class MemMetadataSet : public MetadataSet {
 public:
  MemMetadataSet(ByteArray p_metadata, ByteArray p_offsets, SizeType p_count);

  ~MemMetadataSet();

  ByteArray GetMetadata(IndexType p_vectorID) const;

  SizeType Count() const;

  bool Available() const;

  void AddBatch(MetadataSet& data);

  ErrorCode SaveMetadata(const std::string& p_metaFile,
                         const std::string& p_metaindexFile);

 private:
  std::vector<std::uint64_t> m_offsets;

  SizeType m_count;

  ByteArray m_metadataHolder;

  ByteArray m_offsetHolder;

  std::vector<std::uint8_t> m_newdata;
};

}  // namespace vsearch

#endif //VECTORSEARCH_VSEARCH_METADATA_SET_H_
