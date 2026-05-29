cmake_minimum_required(VERSION 3.15)
project(test_package CXX)

find_package(ninjahsm CONFIG REQUIRED)

add_executable(example src/example.cpp)
target_link_libraries(example PRIVATE ninjahsm::ninjahsm)
target_compile_features(example PRIVATE cxx_std_17)
