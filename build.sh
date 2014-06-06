#!/bin/bash


BUILD_DIR=./build
SRC_DIR=../src


cmake_params="-DPROJECT_SOURCE_DIR=${SRC_DIR}"
cmake_params="${cmake_params} -DCMAKE_BUILD_TYPE=Debug "
#cmake_params="${cmake_params} -DCMAKE_BUILD_TYPE=Release "

rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd ${BUILD_DIR}
#echo "cmake ${cmake_params} ${SRC_DIR}"
cmake ${cmake_params} ${SRC_DIR}
make -j 12 && ./apps/dblayer_test dblayer:sqlite:../examples/test.db
cd ..


