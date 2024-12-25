#!/bin/bash

set -e

mkdir -p build
cd build
cmake ..
make

# Run tests
./test/hello_test