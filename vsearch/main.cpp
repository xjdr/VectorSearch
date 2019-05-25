#include <iostream>
#include <thread>
#include <memory>
#include <chrono>

#include "../extern/glog/glog/logging.h"
#include "../extern/glog/src/glog/log_severity.h"

#include "VectorDB.h"
#include "index_server.h"
#include "vsearch_server.h"


int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "Starting VectorSearch .... ";

  auto db = std::make_shared<VectorDB>();

  std::thread tv([db](){
    VsearchServer vserver(db);
    vserver.Run();
  });

  std::thread ti([db](){
    IndexServer iserver(db);
    iserver.Run();
  });

  ti.join();
  tv.join();

  return 0;
}
