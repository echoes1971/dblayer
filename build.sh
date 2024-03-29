#!/bin/bash


BUILD_DIR=./build
SRC_DIR=../src
#PATH=/home/roberto/Qt/5.4/gcc_64/bin/:${PATH}
#PATH=~/Qt/5.4/clang_64/bin/:~/Qt/5.4/gcc_64/bin/:${PATH}


#CONNECTION_STRING="dblayer:sqlite:../examples/test.db"
#CONNECTION_STRING="dblayer:sqlite:../examples/test_auth.db"
#CONNECTION_STRING="dblayer:mysql:host=localhost;dbname=rproject;user=root;password=;"
#CONNECTION_STRING="dblayer:pg:host=localhost dbname=rproject user=postgres password="
#CONNECTION_STRING="dblayer:qxmlrpc:http://127.0.0.1/~roberto/rp/xmlrpc_server.php"
CONNECTION_STRING="dblayer:qsqlite:../examples/test_auth.db"
TEST_USER=adm
TEST_PWD=adm

echo "PATH: ${PATH}"

cmake_params="-DPROJECT_SOURCE_DIR=${SRC_DIR}"
cmake_params="${cmake_params} -DCMAKE_BUILD_TYPE=Debug "
cmake_params="${cmake_params} -DCMAKE_VERBOSE_MAKEFILE=ON "
#cmake_params="${cmake_params} -DCMAKE_BUILD_TYPE=Release "
#cmake_params="${cmake_params} -DHAVE_CONFIG_H=true "

echo "cmake ${cmake_params} ${SRC_DIR}"
echo "cd build ; make && ./tests/connection_test \"$CONNECTION_STRING\" $TEST_USER $TEST_PWD ; cd .."
echo "cd build ; make && ./tests/dbmgr_test \"$CONNECTION_STRING\" $TEST_USER $TEST_PWD ; cd .."
#exit 1

#rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd ${BUILD_DIR}
cmake ${cmake_params} ${SRC_DIR}
#exit 1

#make -j 12 && ./apps/dbshell dblayer:qxmlrpc:http://127.0.0.1/~roberto/rp/xmlrpc_server.php
#make -j 12 && ./apps/dbshell "dblayer:mysql:host=localhost;dbname=rproject;user=root;password=;" adm adm

make \
    && ./tests/cm_test "$CONNECTION_STRING" $TEST_USER $TEST_PWD \
#    && ./apps/dbshell "$CONNECTION_STRING" \
#    &&./tests/connection_test "$CONNECTION_STRING" $TEST_USER $TEST_PWD \
#    && ./tests/dbmgr_test "$CONNECTION_STRING" $TEST_USER $TEST_PWD \
echo "cd build ; ./tests/cm_test \"$CONNECTION_STRING\" $TEST_USER $TEST_PWD ; cd .."

cd ..

#cd build/ ; make && ./tests/dblayer_test ; cd ..
