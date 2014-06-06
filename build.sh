#!/bin/bash


BUILD_DIR=./build
SRC_DIR=..

mkdir -p $BUILD_DIR
cd BUILD_DIR
cmake $SRC_DIR
make && ./src/dblayer_test dblayer:sqlite:./examples/test.db
cd ..


