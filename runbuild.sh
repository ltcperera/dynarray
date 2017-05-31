#!/usr/bin/env sh
export BUILD_DIR=./build
export CUR_DIR=$(pwd)
mkdir -p $BUILD_DIR

cd $BUILD_DIR
cmake ..
make
cd $CUR_DIR