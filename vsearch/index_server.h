#ifndef VECTORSEARCH_VSEARCH_INDEX_SERVER_H_
#define VECTORSEARCH_VSEARCH_INDEX_SERVER_H_

#include <memory>
#include <iostream>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

#include "metadata_set.h"
#include "vsearch.pb.h"
#include "vsearch.grpc.pb.h"
#include "vector_index.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerCompletionQueue;
using grpc::Status;
using vsearch::Index;
using vsearch::RetCode;
using vsearch::Code;
using vsearch::VectorSearch;
using vsearch::VectorIndex;

class IndexServer final {
 public:
 IndexServer(std::shared_ptr<VectorIndex> index) : index_(index) {

  }
  ~IndexServer() {
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    cq_->Shutdown();
  }

  // There is no shutdown handling in this code.
  void Run() {
    std::string server_address("0.0.0.0:50052");

    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service_" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *asynchronous* service.
    builder.RegisterService(&service_);
    // Get hold of the completion queue used for the asynchronous communication
    // with the gRPC runtime.
    cq_ = builder.AddCompletionQueue();
    // Finally assemble the server.
    server_ = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;

    // Proceed to the server's main loop.
    HandleRpcs();
  }

 private:
  // Class encompasing the state and logic needed to serve a request.
  class CallData {
   public:
    // Take in the "service" instance (in this case representing an asynchronous
    // server) and the completion queue "cq" used for asynchronous communication
    // with the gRPC runtime.
    CallData(VectorSearch::AsyncService* service, ServerCompletionQueue* cq, std::shared_ptr<VectorIndex> index)
        : service_(service), cq_(cq), index_(index), responder_(&ctx_), status_(CREATE) {
      // Invoke the serving logic right away.
      Proceed();
    }

    void Proceed() {
      if (status_ == CREATE) {
        // Make this instance progress to the PROCESS state.
        status_ = PROCESS;
        service_->Requestindex(&ctx_, &request_, &responder_, cq_, cq_, this);
      } else if (status_ == PROCESS) {
        new CallData(service_, cq_, index_);
        
        // The actual processing.
        int m = index_->GetFeatureDim();
        for (auto t : request_.index()) {
          metaoffset.push_back(t.metadata_size());
          std::vector<char> meta(t.metadata().begin(), t.metadata().end());
          std::shared_ptr<vsearch::VectorSet> vecset(new vsearch::BasicVectorSet(
                                                       vsearch::ByteArray((std::uint8_t*)t.data().c_str(), t.n() * m * sizeof(float), false),
                                                       vsearch::GetEnumValueType<float>(), m, t.n()));
          std::shared_ptr<vsearch::MetadataSet> metaset(new vsearch::MemMetadataSet(
                                                       vsearch::ByteArray((std::uint8_t*)meta.data(), meta.size() * sizeof(char), false),
                                                       vsearch::ByteArray((std::uint8_t*)metaoffset.data(),metaoffset.size() * sizeof(long long), false),
                                                       t.n()));
          
          index_->AddIndex(vecset, metaset);
          index_->SaveIndex("test");
        }

        reply_.set_state(vsearch::Code::kOk);
        reply_.set_msg("ok");

        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this);
      } else {
        GPR_ASSERT(status_ == FINISH);
        
        delete this;
      }
    }

   private:
    std::shared_ptr<VectorIndex> index_;
    // The means of communication with the gRPC runtime for an asynchronous
    // server.
    VectorSearch::AsyncService* service_;
    // The producer-consumer queue where for asynchronous server notifications.
    ServerCompletionQueue* cq_;
    // Context for the rpc, allowing to tweak aspects of it such as the use
    // of compression, authentication, as well as to send metadata back to the
    // client.
    ServerContext ctx_;

    // What we get from the client.
    Index request_;
    // What we send back to the client.
    RetCode reply_;

    // The means to get back to the client.
    ServerAsyncResponseWriter<RetCode> responder_;

    // Let's implement a tiny state machine with the following states.
    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_;  // The current serving state.
    std::vector<long long> metaoffset;
  };

  // This can be run in multiple threads if needed.
  void HandleRpcs() {
    // Spawn a new CallData instance to serve new clients.
    new CallData(&service_, cq_.get(), index_);
    void* tag;  // uniquely identifies a request.
    bool ok;
    while (true) {
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      // The return value of Next should always be checked. This return value
      // tells us whether there is any kind of event or cq_ is shutting down.
      GPR_ASSERT(cq_->Next(&tag, &ok));
      GPR_ASSERT(ok);
      static_cast<CallData*>(tag)->Proceed();
    }
  }

  std::shared_ptr<VectorIndex> index_;
  std::unique_ptr<ServerCompletionQueue> cq_;
  VectorSearch::AsyncService service_;
  std::unique_ptr<Server> server_;
};

#endif //VECTORSEARCH_VSEARCH_INDEX_SERVER_H_
