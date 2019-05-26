// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "neighborhoodgraph.h"
#include "relativeneighborhoodgraph.h"

using namespace vsearch::COMMON;

std::shared_ptr<NeighborhoodGraph> NeighborhoodGraph::CreateInstance(
    std::string type) {
  std::shared_ptr<NeighborhoodGraph> res;
  if (type == "RNG") {
    res.reset(new RelativeNeighborhoodGraph);
  }
  return res;
}