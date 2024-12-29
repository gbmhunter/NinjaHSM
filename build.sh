#!/bin/bash

set -e

mkdir -p build
cd build
cmake ..
make

# Run tests
# ./test/standard/hello_test --gtest_filter="HsmTests.CanTransitionToChildStateFromParentEntry"
./test/standard/hello_test
exit 0

cd ../test/cmake_fetch_content
mkdir -p build
cd build
cmake ..
cmake --build .
./cmake_fetch_content
