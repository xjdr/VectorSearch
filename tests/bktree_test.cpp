#include "gtest/gtest.h"

#include <iostream>
#include <fstream>
#include <string>

#include "../vsearch/bktree.h"

using vsearch::COMMON::BKTree;

class BKTreeTest : public testing::Test {

};

TEST(BKTreeTest, BKTree_Basic_Test) {
  BKTree bkt;

  //bkt.InitSearchTrees()
  //bkt.BuildTrees()
  //bkt.SearchTrees()
}