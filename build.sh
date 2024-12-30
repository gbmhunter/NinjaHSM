#!/bin/bash

set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

mkdir -p build
cd build
cmake ..
make

# Run tests
# ./test/standard/hello_test --gtest_filter="HsmTests.CanTransitionToChildStateFromParentEntry"
./test/standard/hello_test
# exit 0

cd $SCRIPT_DIR/test/cmake_fetch_content
mkdir -p build
cd build
cmake ..
cmake --build .
./cmake_fetch_content

# Build the basic example
cd $SCRIPT_DIR/examples/basic_example
mkdir -p build
cd build
cmake ..
cmake --build .
./basic_example