// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: vsearch.proto

#include "vsearch.pb.h"
#include "vsearch.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace vsearch {

static const char* VectorSearch_method_names[] = {
  "/vsearch.VectorSearch/search",
};

std::unique_ptr< VectorSearch::Stub> VectorSearch::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< VectorSearch::Stub> stub(new VectorSearch::Stub(channel));
  return stub;
}

VectorSearch::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_search_(VectorSearch_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status VectorSearch::Stub::search(::grpc::ClientContext* context, const ::vsearch::SearchRequest& request, ::vsearch::SearchResponse* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_search_, context, request, response);
}

void VectorSearch::Stub::experimental_async::search(::grpc::ClientContext* context, const ::vsearch::SearchRequest* request, ::vsearch::SearchResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_search_, context, request, response, std::move(f));
}

void VectorSearch::Stub::experimental_async::search(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::vsearch::SearchResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall(stub_->channel_.get(), stub_->rpcmethod_search_, context, request, response, std::move(f));
}

void VectorSearch::Stub::experimental_async::search(::grpc::ClientContext* context, const ::vsearch::SearchRequest* request, ::vsearch::SearchResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_search_, context, request, response, reactor);
}

void VectorSearch::Stub::experimental_async::search(::grpc::ClientContext* context, const ::grpc::ByteBuffer* request, ::vsearch::SearchResponse* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create(stub_->channel_.get(), stub_->rpcmethod_search_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::vsearch::SearchResponse>* VectorSearch::Stub::AsyncsearchRaw(::grpc::ClientContext* context, const ::vsearch::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::vsearch::SearchResponse>::Create(channel_.get(), cq, rpcmethod_search_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::vsearch::SearchResponse>* VectorSearch::Stub::PrepareAsyncsearchRaw(::grpc::ClientContext* context, const ::vsearch::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::vsearch::SearchResponse>::Create(channel_.get(), cq, rpcmethod_search_, context, request, false);
}

VectorSearch::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      VectorSearch_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< VectorSearch::Service, ::vsearch::SearchRequest, ::vsearch::SearchResponse>(
          std::mem_fn(&VectorSearch::Service::search), this)));
}

VectorSearch::Service::~Service() {
}

::grpc::Status VectorSearch::Service::search(::grpc::ServerContext* context, const ::vsearch::SearchRequest* request, ::vsearch::SearchResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace vsearch

