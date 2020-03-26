add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "../extern/googletest")

mark_as_advanced(
        BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
        gmock_build_tests gtest_build_samples gtest_build_tests
        gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} 
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/librocksdb.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libtbb.so
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libprotobuf.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/librocksdb.a
  PUBLIC ${PROJECT_SOURCE_DIR}/lib/libtbb.so
  gtest
  gmock
  gtest_main
  -ldl
  -pthread
  -lbz2
  -lc
  -lcrypto
  -lssl
  -lz
  -lcares
      )
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

include_directories(${PROJECT_SOURCE_DIR}/vsearch)

package_add_test(distance_test distance_test.cpp
        ../vsearch/vsearch.pb.h
        ../vsearch/vsearch.pb.cc
        ../vsearch/distance.h
        )

package_add_test(bktree_test bktree_test.cpp
        ../vsearch/bktree.h
        )

package_add_test(vsearch_test vsearch_test.cpp
        #../vsearch/vectordb.cpp ../vsearch/vectordb
        ../vsearch/distance.h ../vsearch/common.h ../vsearch/definition_list.h ../vsearch/common_utils.h ../vsearch/neighborhoodgraph.h
        ../vsearch/neighborhoodgraph.cpp ../vsearch/relativeneighborhoodgraph.h ../vsearch/vector_index.h ../vsearch/vector_index.cpp
        ../vsearch/search_query.h ../vsearch/bktree.h ../vsearch/workspace.h ../vsearch/heap.h ../vsearch/query_result_set.h ../vsearch/kdtree.h
        ../vsearch/vector_set.cpp ../vsearch/fine_grained_lock.h ../vsearch/common_helper.cpp
        ../vsearch/vector_set.h ../vsearch/common_data_structure.h ../vsearch/common_data_structure.cpp ../vsearch/metadata_set.h
        ../vsearch/metadata_set.cpp ../vsearch/bkt_index.h ../vsearch/bkt_idex.cpp ../vsearch/simple_ini_reader.h ../vsearch/string_converter.h
        ../vsearch/bkt_parameter_definition_list.h ../vsearch/dataset.h ../vsearch/workspace_pool.h ../vsearch/workspace_pool.cpp
        ../vsearch/common_helper.h ../vsearch/simple_ini_reader.cpp
        ../vsearch/data_utils.h ../vsearch/kdt_index.h ../vsearch/kdt_parameter_definition_list.h ../vsearch/kdt_index.cpp
        )



target_include_directories(distance_test
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  )

target_include_directories(bktree_test
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  )

target_include_directories(vsearch_test
  PUBLIC ${PROJECT_SOURCE_DIR}/include
  )

add_library(tbb SHARED IMPORTED)
set_property(TARGET tbb PROPERTY IMPORTED_LOCATION PUBLIC "${PROJECT_SOURCE_DIR}/lib/libtbb.so.2")

add_library(cares SHARED IMPORTED)
set_property(TARGET cares PROPERTY IMPORTED_LOCATION PUBLIC "${PROJECT_SOURCE_DIR}/lib/libcares.so.2")

