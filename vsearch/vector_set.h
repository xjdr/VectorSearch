// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef VECTORSEARCH_VSEARCH_VECTOR_SET_H_
#define VECTORSEARCH_VSEARCH_VECTOR_SET_H_

#include "common_data_structure.h"

namespace vsearch {

class VectorSet {
 public:
  VectorSet();

  virtual ~VectorSet();

  virtual VectorValueType GetValueType() const = 0;

  virtual void* GetVector(IndexType p_vectorID) const = 0;

  virtual void* GetData() const = 0;

  virtual SizeType Dimension() const = 0;

  virtual SizeType Count() const = 0;

  virtual bool Available() const = 0;

  virtual ErrorCode Save(const std::string& p_vectorFile) const = 0;
};

class BasicVectorSet : public VectorSet {
 public:
  BasicVectorSet(const ByteArray& p_bytesArray, VectorValueType p_valueType,
                 SizeType p_dimension, SizeType p_vectorCount);

  virtual ~BasicVectorSet();

  virtual VectorValueType GetValueType() const;

  virtual void* GetVector(IndexType p_vectorID) const;

  virtual void* GetData() const;

  virtual SizeType Dimension() const;

  virtual SizeType Count() const;

  virtual bool Available() const;

  virtual ErrorCode Save(const std::string& p_vectorFile) const;

 private:
  ByteArray m_data;

  VectorValueType m_valueType;

  SizeType m_dimension;

  SizeType m_vectorCount;

  SizeType m_perVectorDataSize;
};

}  // namespace vsearch


#endif //VECTORSEARCH_VSEARCH_VECTOR_SET_H_
