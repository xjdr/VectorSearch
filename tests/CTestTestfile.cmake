# CMake generated Testfile for 
# Source directory: /home/xjdr/src/xjdr/VectorSearch/tests
# Build directory: /home/xjdr/src/xjdr/VectorSearch/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(distance_test "/home/xjdr/src/xjdr/VectorSearch/tests/distance_test")
set_tests_properties(distance_test PROPERTIES  _BACKTRACE_TRIPLES "/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;17;add_test;/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;21;package_add_test;/home/xjdr/src/xjdr/VectorSearch/tests/CMakeLists.txt;0;")
subdirs("../extern/googletest")
