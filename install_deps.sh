#tbb
make -j4
cp build/linux_*/*.so* ../../lib/
cp -R include/* ../../include/

#rocksdb
make static_lib -j4
cp -r include/* ../../include/
cp librocksdb.a ../../lib/

# absl
mkdir -p cmake/build
cd cmake/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DCMAKE_INSTALL_PREFIX="$(pwd)/../../../../" ../..
make install -j4

#c-ares
mkdir -p cmake/build
cd cmake/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$(pwd)/../../../../" ../..
make install -j4

#protobuf
./autogen
./configure --prefix=$(pwd)/../../
make -j4
make install

#gRPC
mkdir -p cmake/build
cd cmake/build
cmake -E env CXXFLAGS="" cmake -DCMAKE_PREFIX_PATH=$(pwd)/../../../../ \
      -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DgRPC_ABSL_PROVIDER=package \
      -DgRPC_CARES_PROVIDER=package \
      -DgRPC_PROTOBUF_PROVIDER=package \
      -DgRPC_SSL_PROVIDER=package \
      -DgRPC_ZLIB_PROVIDER=package \
      -DProtobuf_INCLUDE_DIRS=$(pwd)/../../../../include \
      -DProtobuf_LIBRARIES=$(pwd)/../../../../lib/libprotobuf.a \
      -DProtobuf_PROTOC_LIBRARIES=$(pwd)/../../../../lib/libprotoc.a \
      -DProtobuf_PROTOC_EXECUTABLE=$(pwd)/../../../../bin/protoc \
      ../..

# Build Protos
bin/protoc \
    --grpc_out vsearch/ \
    --cpp_out=vsearch/ \
    -I vsearch/ \
    --plugin=protoc-gen-grpc=bin/grpc_cpp_plugin \
    vsearch/vsearch.proto

# Build Vector Search
mkdir -p cmake/build
cd cmake/build
cmake -E env CXXFLAGS="" cmake -DCMAKE_PREFIX_PATH=../.. \
-DProtobuf_INCLUDE_DIRS=$(pwd)/../../include \
-DProtobuf_LIBRARIES=$(pwd)/../../lib/libprotobuf.a \
-DProtobuf_PROTOC_LIBRARIES=$(pwd)/../../lib/libprotoc.a \
-DProtobuf_PROTOC_EXECUTABLE=$(pwd)/../../bin/protoc \
 ../..

make -j4

