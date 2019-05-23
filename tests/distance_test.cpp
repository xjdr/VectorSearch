#include "gtest/gtest.h"

#include "../third_party/SPTAG/distance.h"
#include "../vsearch/vsearch.pb.h"

#include <iostream>
#include <fstream>
#include <string>

class DistanceTest : public testing::Test {

};

TEST(DistanceTest, DistanceTest_L2_Test_Far) {
  float pX[] = {1.3, 1.3};
  float pY[] = {6.6, 6.2};
  float r = vsearch::Distance::ComputeL2Distance(pX, pY, 2);
  EXPECT_FLOAT_EQ(r, 52.1);
}

TEST(DistanceTest, DistanceTest_L2_Test_Close) {
  float pX[] = {1.3, 1.3};
  float pY[] = {1.29, 1.28};
  float r = vsearch::Distance::ComputeL2Distance(pX, pY, 2);
  EXPECT_FLOAT_EQ(r, 0.00049999903);
}

TEST(DistanceTest, DistanceTest_Cosine_Test_Far) {
  float pX[] = {1.3, 1.3};
  float pY[] = {6.6, 6.2};
  float r = vsearch::Distance::ComputeCosineDistance(pX, pY, 2);
  EXPECT_FLOAT_EQ(r, -15.639999);
}

TEST(DistanceTest, DistanceTest_Cosine_Test_Close) {
  float pX[] = {1.3, 1.3};
  float pY[] = {1.29, 1.28};
  float r = vsearch::Distance::ComputeCosineDistance(pX, pY, 2);
  EXPECT_FLOAT_EQ(r, -2.3409998);
}

TEST(DistanceTest, DistanceTest_Cosine_Test_S2D_Far) {
  float pX[] = {1.3, 1.3};
  float pY[] = {6.6, 6.2};
  float r = vsearch::Distance::ComputeCosineDistance(pX, pY, 2);
  EXPECT_FLOAT_EQ(vsearch::Distance::ConvertCosineSimilarityToDistance(r), 16.639999);
}

TEST(DistanceTest, DistanceTest_Cosine_Test_S2D_Close) {
  float pX[] = {1.3, 1.3};
  float pY[] = {1.29, 1.28};
  float r = vsearch::Distance::ComputeCosineDistance(pX, pY, 2);
  EXPECT_FLOAT_EQ(vsearch::Distance::ConvertCosineSimilarityToDistance(r), 3.3409998);
}

TEST(DistanceTest, DistanceTest_L2_Test_Tensor) {
  vsearch::Index i;
  std::ifstream file;
  std::string s = std::string("/home/xjdr/src/xjdr/VectorSearch/tests/tensor.txt");
  file.open(s.c_str(), std::fstream::in);
  if(!i.ParseFromIstream(&file)) {
    std::cerr << "Boooo" << std::endl;
  }

  float pX[i.index(0).size()];
  float pY[i.index(1).size()];
  std::memcpy(pX, i.index(0).data().c_str(), sizeof(float) * i.index(0).size());
  std::memcpy(pY, i.index(1).data().c_str(), sizeof(float) * i.index(1).size());
  float r = vsearch::Distance::ComputeL2Distance(pX, pY, i.index(0).size());
  EXPECT_FLOAT_EQ(r, 369.71799);
}

TEST(DistanceTest, DistanceTest_Cosine_Test_Tensor) {
  vsearch::Index i;
  std::ifstream file;
  std::string s = std::string("/home/xjdr/src/xjdr/VectorSearch/tests/tensor.txt");
  file.open(s.c_str(), std::fstream::in);
  if(!i.ParseFromIstream(&file)) {
    std::cerr << "Boooo" << std::endl;
  }

  float pX[i.index(0).size()];
  float pY[i.index(1).size()];
  std::memcpy(pX, i.index(0).data().c_str(), sizeof(float) * i.index(0).size());
  std::memcpy(pY, i.index(1).data().c_str(), sizeof(float) * i.index(1).size());
  float r = vsearch::Distance::ComputeCosineDistance(pX, pY, i.index(0).size());
  EXPECT_FLOAT_EQ(r, -1435.8535);
}

TEST(DistanceTest, DistanceTest_CosineDistance_Test_Tensor) {
  vsearch::Index i;
  std::ifstream file;
  std::string s = std::string("/home/xjdr/src/xjdr/VectorSearch/tests/tensor.txt");
  file.open(s.c_str(), std::fstream::in);
  if (!i.ParseFromIstream(&file)) {
    std::cerr << "Boooo" << std::endl;
  }

  float pX[i.index(0).size()];
  float pY[i.index(1).size()];
  std::memcpy(pX, i.index(0).data().c_str(), sizeof(float) * i.index(0).size());
  std::memcpy(pY, i.index(1).data().c_str(), sizeof(float) * i.index(1).size());
  float r = vsearch::Distance::ComputeCosineDistance(pX, pY, i.index(0).size());
  EXPECT_FLOAT_EQ(vsearch::Distance::ConvertCosineSimilarityToDistance(r), 1436.8535);
}