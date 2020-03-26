cmake_minimum_required(VERSION 3.14)
project(VectorSearch)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -fopenmp -I ${PROJECT_SOURCE_DIR}/include -L ${PROJECT_SOURCE_DIR}/lib")

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(extern/glog)

set(_PROTOBUF_LIBPROTOBUF libprotobuf)
set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
set(_GRPC_GRPCPP_UNSECURE grpc++_unsecure)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)

# Proto file
get_filename_component(hw_proto "vsearch/vsearch.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${PROJECT_SOURCE_DIR}/vsearch/vsearch.pb.cc")
set(hw_proto_hdrs "${PROJECT_SOURCE_DIR}/vsearch/vsearch.pb.h")
set(hw_grpc_srcs "${PROJECT_SOURCE_DIR}/vsearch/vsearch.grpc.pb.cc" )
set(hw_grpc_hdrs "${PROJECT_SOURCE_DIR}/vsearch/vsearch.grpc.pb.h")

add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND protoc #${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}/vsearch"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/vsearch"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${PROJECT_SOURCE_DIR}/bin/grpc_cpp_plugin" #"${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")

include_directories(${PROJECT_SOURCE_DIR}/vsearch)

add_executable(VectorSearch
  vsearch/main.cpp
  vsearch/vsearch_server.h
  vsearch/index_server.h
  vsearch/vectordb.cpp
  vsearch/vectordb.h
  vsearch/distance.h
  vsearch/common.h
  vsearch/definition_list.h
  vsearch/common_utils.h
  vsearch/neighborhoodgraph.h
  vsearch/neighborhoodgraph.cpp
  vsearch/relativeneighborhoodgraph.h
  vsearch/vector_index.h
  vsearch/vector_index.cpp
  vsearch/search_query.h
  vsearch/bktree.h
  vsearch/workspace.h
  vsearch/heap.h
  vsearch/query_result_set.h
  vsearch/kdtree.h
  vsearch/vector_set.cpp
  vsearch/fine_grained_lock.h
  vsearch/common_helper.cpp
  vsearch/vector_set.h
  vsearch/common_data_structure.h
  vsearch/common_data_structure.cpp
  vsearch/metadata_set.h
  vsearch/metadata_set.cpp
  vsearch/bkt_index.h
  vsearch/bkt_idex.cpp
  vsearch/simple_ini_reader.h
  vsearch/string_converter.h
  vsearch/bkt_parameter_definition_list.h
  vsearch/dataset.h
  vsearch/workspace_pool.h
  vsearch/workspace_pool.cpp
  vsearch/common_helper.h
  vsearch/simple_ini_reader.cpp
  vsearch/data_utils.h
  vsearch/kdt_index.h
  vsearch/kdt_parameter_definition_list.h
  vsearch/kdt_index.cpp
  ${hw_proto_srcs}
  ${hw_grpc_srcs}
  )

target_include_directories(VectorSearch
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  )

add_library(tbb SHARED IMPORTED)
set_property(TARGET tbb PROPERTY IMPORTED_LOCATION PUBLIC "${PROJECT_SOURCE_DIR}/lib/libtbb.so.2")

add_library(cares SHARED IMPORTED)
set_property(TARGET cares PROPERTY IMPORTED_LOCATION PUBLIC "${PROJECT_SOURCE_DIR}/lib/libcares.so.2")

target_link_libraries(VectorSearch
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/librocksdb.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libtbb.so

  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgrpc++_alts.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgrpc++_error_details.a

  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgrpcpp_channelz.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgrpc++_reflection.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgrpc++_unsecure.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgrpc++.a

  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libprotobuf.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgrpc_plugin_support.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgrpc.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libupb.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libgpr.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libaddress_sorting.a   

  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_bad_optional_access.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_time.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_time_zone.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_str_format_internal.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_strings.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_synchronization.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_strings_internal.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_throw_delegate.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_raw_logging_internal.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_int128.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libabsl_base.a

  -ldl
  -pthread
  -lbz2
  -lc
  -lcrypto
  -lssl
  -lz
  -lcares
#  -ltbb
   glog::glog
   )
