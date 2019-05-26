#include "gtest/gtest.h"

#include "../vsearch/common_data_structure.h"
#include "../vsearch/vector_set.h"
#include "../vsearch/vector_index.h"

class VectorSearchTest : public testing::Test {
 public:
  template <typename T>
  void Build(vsearch::IndexAlgoType algo, std::string distCalcMethod, T* vec, int n,
             int m) {
    std::vector<char> meta;
    std::vector<long long> metaoffset;
    for (int i = 0; i < n; i++) {
      metaoffset.push_back(meta.size());
      std::string a = std::to_string(i);
      for (int j = 0; j < a.length(); j++) meta.push_back(a[j]);
    }
    metaoffset.push_back(meta.size());

    std::shared_ptr<vsearch::VectorSet> vecset(new vsearch::BasicVectorSet(
        vsearch::ByteArray((std::uint8_t*)vec, n * m * sizeof(T), false),
        vsearch::GetEnumValueType<T>(), m, n));

    std::shared_ptr<vsearch::MetadataSet> metaset(new vsearch::MemMetadataSet(
        vsearch::ByteArray((std::uint8_t*)meta.data(), meta.size() * sizeof(char),
                           false),
        vsearch::ByteArray((std::uint8_t*)metaoffset.data(),
                           metaoffset.size() * sizeof(long long), false),
        n));
    std::shared_ptr<vsearch::VectorIndex> vecIndex =
        vsearch::VectorIndex::CreateInstance(algo, vsearch::GetEnumValueType<T>());
    vecIndex->SetParameter("DistCalcMethod", distCalcMethod);
    EXPECT_TRUE(nullptr != vecIndex);
    EXPECT_TRUE(vsearch::ErrorCode::Success == vecIndex->BuildIndex(vecset, metaset));
    EXPECT_TRUE(vsearch::ErrorCode::Success == vecIndex->SaveIndex("origindices"));
  }

  template <typename T>
  void Search(std::string folder, T* vec, int k) {
    std::shared_ptr<vsearch::VectorIndex> vecIndex;
    EXPECT_TRUE(vsearch::ErrorCode::Success == vsearch::VectorIndex::LoadIndex(folder, vecIndex));
    EXPECT_TRUE(nullptr != vecIndex);

    vsearch::QueryResult res(vec, k, true);
    vecIndex->SearchIndex(res);
    for (int i = 0; i < k; i++) {
      std::cout << res.GetResult(i)->Dist << "@(" << res.GetResult(i)->VID << ","
                << std::string((char*)res.GetMetadata(i).Data(),
                               res.GetMetadata(i).Length())
                << ") ";
    }
    std::cout << std::endl;
    vecIndex.reset();
  }

  template <typename T>
  void Add(T* vec, int n) {
    std::shared_ptr<vsearch::VectorIndex> vecIndex;
    EXPECT_TRUE(vsearch::ErrorCode::Success == vsearch::VectorIndex::LoadIndex("origindices", vecIndex));
    EXPECT_TRUE(nullptr != vecIndex);

    std::vector<char> meta;
    std::vector<long long> metaoffset;
    for (int i = 0; i < n; i++) {
      metaoffset.push_back(meta.size());
      std::string a = std::to_string(vecIndex->GetNumSamples() + i);
      for (int j = 0; j < a.length(); j++) meta.push_back(a[j]);
    }
    metaoffset.push_back(meta.size());

    int m = vecIndex->GetFeatureDim();
    std::shared_ptr<vsearch::VectorSet> vecset(new vsearch::BasicVectorSet(
        vsearch::ByteArray((std::uint8_t*)vec, n * m * sizeof(T), false),
        vsearch::GetEnumValueType<T>(), m, n));

    std::shared_ptr<vsearch::MetadataSet> metaset(new vsearch::MemMetadataSet(
        vsearch::ByteArray((std::uint8_t*)meta.data(), meta.size() * sizeof(char),
                           false),
        vsearch::ByteArray((std::uint8_t*)metaoffset.data(),
                           metaoffset.size() * sizeof(long long), false),
        n));

    EXPECT_TRUE(vsearch::ErrorCode::Success == vecIndex->AddIndex(vecset, metaset));
    EXPECT_TRUE(vsearch::ErrorCode::Success == vecIndex->SaveIndex("addindices"));
    vecIndex.reset();
  }

  template <typename T>
  void Delete(T* vec, int n) {
    std::shared_ptr<vsearch::VectorIndex> vecIndex;
    EXPECT_TRUE(vsearch::ErrorCode::Success == vsearch::VectorIndex::LoadIndex("addindices", vecIndex));
    EXPECT_TRUE(nullptr != vecIndex);

    EXPECT_TRUE(vsearch::ErrorCode::Success == vecIndex->DeleteIndex((const void*)vec, n));
    EXPECT_TRUE(vsearch::ErrorCode::Success == vecIndex->SaveIndex("delindices"));
    vecIndex.reset();
  }

  template <typename T>
  void Test(vsearch::IndexAlgoType algo, std::string distCalcMethod) {
    int n = 100, q = 3, m = 10, k = 3;
    std::vector<T> vec;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        vec.push_back((T)i);
      }
    }

    std::vector<T> query;
    for (int i = 0; i < q; i++) {
      for (int j = 0; j < m; j++) {
        query.push_back((T)i * 2);
      }
    }

    Build<T>(algo, distCalcMethod, vec.data(), n, m);
    Search<T>("origindices", query.data(), k);
    Add<T>(query.data(), q);
    Search<T>("addindices", query.data(), k);
    Delete<T>(query.data(), q);
    Search<T>("delindices", query.data(), k);
  }
};

TEST_F(VectorSearchTest, VectorSearchTest_KDTTest) {
 Test<float>(vsearch::IndexAlgoType::KDT, "L2");
}

TEST_F(VectorSearchTest, VectorSearchTest_BKTTest) {
  Test<float>(vsearch::IndexAlgoType::BKT, "L2");
}

