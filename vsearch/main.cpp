#include <iostream>
#include <thread>
#include <memory>
#include <chrono>

#include "glog/logging.h"
#include "glog/log_severity.h"

#include "vectordb.h"
#include "index_server.h"
#include "vsearch_server.h"

#include "common_data_structure.h"
#include "vector_set.h"
#include "vector_index.h"
#include "bkt_index.h"

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "Starting VectorSearch .... ";

  //auto db = std::make_shared<VectorDB>();
  
  // If index exists, load it
  //if (vsearch::ErrorCode::Success == vsearch::VectorIndex::LoadIndex(<LOCATION>, vecIndex)) {

  // If not, build one
  //} else {
  auto vecIndex = vsearch::VectorIndex::CreateInstance(vsearch::IndexAlgoType::BKT, vsearch::GetEnumValueType<float>());
  vecIndex->SetParameter("DistCalcMethod", "L2");
  //}
  
  std::thread tv([vecIndex](){
    VsearchServer vserver(vecIndex);
    vserver.Run();
  });

  std::thread ti([vecIndex](){
    IndexServer iserver(vecIndex);
    iserver.Run();
  });

  ti.join();
  tv.join();

  return 0;
}
