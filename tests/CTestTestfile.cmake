# CMake generated Testfile for 
# Source directory: /home/xjdr/src/xjdr/VectorSearch/tests
# Build directory: /home/xjdr/src/xjdr/VectorSearch/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(distance_test "/home/xjdr/src/xjdr/VectorSearch/tests/distance_test")
set_tests_properties(distance_test PROPERTIES  _BACKTRACE_TRIPLES "/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;17;add_test;/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;23;package_add_test;/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;0;")
add_test(bktree_test "/home/xjdr/src/xjdr/VectorSearch/tests/bktree_test")
set_tests_properties(bktree_test PROPERTIES  _BACKTRACE_TRIPLES "/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;17;add_test;/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;29;package_add_test;/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;0;")
add_test(vsearch_test "/home/xjdr/src/xjdr/VectorSearch/tests/vsearch_test")
set_tests_properties(vsearch_test PROPERTIES  _BACKTRACE_TRIPLES "/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;17;add_test;/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;33;package_add_test;/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;0;")
subdirs("../extern/googletest")
