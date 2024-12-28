#!/bin/bash

set -e

mkdir -p build
cd build
cmake ..
make

# Run tests
# ./test/hello_test --gtest_filter="HsmTests.EventsBubbleUpToParentStates"
./test/standard/hello_test

cd ../test/cmake_fetch_content
mkdir -p build
cd build
cmake ..
cmake --build .
./cmake_fetch_content
