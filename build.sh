#!/bin/bash

set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

mkdir -p build
cd build

# Pass in option to build tests
cmake .. -DNINJAHSM_BUILD_TESTS=ON
cmake --build .

# Run tests
# ./test/standard/hello_test --gtest_filter="HsmTests.CanTransitionToChildStateFromParentEntry"
./test/tests

# Build the basic example
cd $SCRIPT_DIR/examples/basic_example
mkdir -p build
cd build
cmake ..
cmake --build .
./basic_example