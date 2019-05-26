// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef VECTORSEARCH_VSEARCH_BKT_INDEX_H_
#define VECTORSEARCH_VSEARCH_BKT_INDEX_H_

#include "common.h"
#include "vector_index.h"
#include "bktree.h"

#include "common_utils.h"
#include "dataset.h"
#include "distance.h"
#include "query_result_set.h"
#include "relativeneighborhoodgraph.h"
#include "workspace.h"
#include "workspace_pool.h"
#include "simple_ini_reader.h"
#include "string_converter.h"

#include <tbb/concurrent_unordered_set.h>
#include <functional>
#include <mutex>

namespace vsearch {

namespace Helper {
class IniReader;
}

namespace BKT {
template <typename T>
class Index : public VectorIndex {
 private:
  // data points
  COMMON::Dataset<T> m_pSamples;

  // BKT structures.
  COMMON::BKTree m_pTrees;

  // Graph structure
  COMMON::RelativeNeighborhoodGraph m_pGraph;

  std::string m_sBKTFilename;
  std::string m_sGraphFilename;
  std::string m_sDataPointsFilename;

  std::mutex m_dataLock;  // protect data and graph
  tbb::concurrent_unordered_set<int> m_deletedID;
  std::unique_ptr<COMMON::WorkSpacePool> m_workSpacePool;

  int m_iNumberOfThreads;
  DistCalcMethod m_iDistCalcMethod;
  float (*m_fComputeDistance)(const T* pX, const T* pY, int length);

  int m_iMaxCheck;
  int m_iThresholdOfNumberOfContinuousNoBetterPropagation;
  int m_iNumberOfInitialDynamicPivots;
  int m_iNumberOfOtherDynamicPivots;

 public:
  Index() {
#define DefineBKTParameter(VarName, VarType, DefaultValue, RepresentStr) \
  VarName = DefaultValue;

#include "bkt_parameter_definition_list.h"
#undef DefineBKTParameter

    m_fComputeDistance = DistanceCalcSelector<T>(m_iDistCalcMethod);
  }

  ~Index() {}

  inline int GetNumSamples() const { return m_pSamples.R(); }
  inline int GetFeatureDim() const { return m_pSamples.C(); }

  inline int GetCurrMaxCheck() const { return m_iMaxCheck; }
  inline int GetNumThreads() const { return m_iNumberOfThreads; }
  inline DistCalcMethod GetDistCalcMethod() const { return m_iDistCalcMethod; }
  inline IndexAlgoType GetIndexAlgoType() const { return IndexAlgoType::BKT; }
  inline VectorValueType GetVectorValueType() const {
    return GetEnumValueType<T>();
  }

  inline float ComputeDistance(const void* pX, const void* pY) const {
    return m_fComputeDistance((const T*)pX, (const T*)pY, m_pSamples.C());
  }
  inline const void* GetSample(const int idx) const {
    return (void*)m_pSamples[idx];
  }

  ErrorCode BuildIndex(const void* p_data, int p_vectorNum, int p_dimension);

  ErrorCode LoadIndexFromMemory(const std::vector<void*>& p_indexBlobs);

  ErrorCode SaveIndex(const std::string& p_folderPath,
                      std::ofstream& p_configout);
  ErrorCode LoadIndex(const std::string& p_folderPath,
                      Helper::IniReader& p_reader);
  ErrorCode SearchIndex(QueryResult& p_query) const;
  ErrorCode AddIndex(const void* p_vectors, int p_vectorNum, int p_dimension);
  ErrorCode DeleteIndex(const void* p_vectors, int p_vectorNum);

  ErrorCode SetParameter(const char* p_param, const char* p_value);
  std::string GetParameter(const char* p_param) const;

 private:
  ErrorCode RefineIndex(const std::string& p_folderPath);
  void SearchIndexWithDeleted(
      COMMON::QueryResultSet<T>& p_query, COMMON::WorkSpace& p_space,
      const tbb::concurrent_unordered_set<int>& p_deleted) const;
  void SearchIndexWithoutDeleted(COMMON::QueryResultSet<T>& p_query,
                                 COMMON::WorkSpace& p_space) const;
};
}  // namespace BKT
}  // namespace vsearch

#endif //VECTORSEARCH_VSEARCH_BKT_INDEX_H_
