#!/bin/bash


BUILD_DIR=./build
SRC_DIR=../src
PATH=/home/roberto/Qt/5.3/gcc_64/bin/:${PATH}
PATH=~/Qt/5.3/clang_64/bin/:~/Qt/5.3/gcc_64/bin/:${PATH}

echo "PATH: ${PATH}"

cmake_params="-DPROJECT_SOURCE_DIR=${SRC_DIR}"
cmake_params="${cmake_params} -DCMAKE_BUILD_TYPE=Debug "
#cmake_params="${cmake_params} -DCMAKE_BUILD_TYPE=Release "
#cmake_params="${cmake_params} -DHAVE_CONFIG_H=true "


rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd ${BUILD_DIR}
echo "cmake ${cmake_params} ${SRC_DIR}"
#exit 1
cmake ${cmake_params} ${SRC_DIR}
#make && ./apps/dblayer_test dblayer:sqlite:../examples/test.db
#make -j 12 && ./apps/dblayer_test dblayer:sqlite:../examples/test.db
#make -j 12 && ./apps/dbshell dblayer:qxmlrpc:http://127.0.0.1/~roberto/rp/xmlrpc_server.php

#make -j 12 && ./apps/dbshell "dblayer:mysql:host=localhost;dbname=rproject;user=root;password=;"

make -j 12 && ./apps/dblayer_test dblayer:qxmlrpc:http://127.0.0.1/~roberto/rp/xmlrpc_server.php roberto echoestrade


cd ..

