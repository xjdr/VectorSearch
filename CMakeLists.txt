cmake_minimum_required(VERSION 3.14)
project(VectorSearch)

set(CMAKE_CXX_STANDARD 14)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Protobuf
include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

add_subdirectory(extern/glog)
add_subdirectory(extern/abseil-cpp)
add_subdirectory(extern/grpc EXCLUDE_FROM_ALL)

set(_PROTOBUF_LIBPROTOBUF libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)

# Proto file
get_filename_component(hw_proto "vsearch/vsearch.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/vsearch/vsearch.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/vsearch/vsearch.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/vsearch/vsearch.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/vsearch/vsearch.grpc.pb.h")

add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}/vsearch"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/vsearch"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")

add_executable(VectorSearch
        vsearch/main.cpp vsearch/vsearch_server.h vsearch/index_server.h
        third_party/SPTAG/distance.h third_party/SPTAG/common.h third_party/SPTAG/definition_list.h third_party/SPTAG/common_utils.h
        ${hw_proto_srcs}
        ${hw_grpc_srcs}
        vsearch/VectorDB.cpp vsearch/VectorDB.h)

target_include_directories(VectorSearch
        PUBLIC ${PROTOBUF_INCLUDE_DIRS}
)

target_link_libraries(VectorSearch
  absl::base absl::synchronization absl::strings
        glog::glog
        -lrocksdb
        ${_GRPC_GRPCPP_UNSECURE}
        ${_PROTOBUF_LIBPROTOBUF}
        )
