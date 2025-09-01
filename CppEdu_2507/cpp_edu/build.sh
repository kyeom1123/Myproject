#!/bin/bash
set -e
BUILD_DIR=build

rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
cmake ..
cmake --build .

cd ..

cp ./$BUILD_DIR/main ./