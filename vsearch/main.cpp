#include <iostream>
#include <glog/logging.h>

#include "vsearch_server.h"

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  LOG(INFO) << "Starting vsearch .... ";

  VsearchServer server;
  server.Run();
}