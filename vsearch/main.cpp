#include <iostream>
#include <thread>
#include <glog/logging.h>

#include "index_server.h"
#include "vsearch_server.h"

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "Starting vsearch .... ";

  std::thread tv([](){
    VsearchServer vserver;
    vserver.Run();
  });

  std::thread ti([](){
    IndexServer iserver;
    iserver.Run();
  });

  tv.join();
  ti.join();

  return 0;
}