#include <iostream>
#include <thread>
#include <memory>
#include <chrono>

//#include "glog/logging.h"
//#include "glog/log_severity.h"

#include "vectordb.h"
#include "index_server.h"
#include "vsearch_server.h"


int main(int argc, char* argv[]) {
  //google::InitGoogleLogging(argv[0]);

  //LOG(INFO) << "Starting VectorSearch .... ";

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
