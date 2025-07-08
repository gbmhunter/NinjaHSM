cmake_minimum_required(VERSION 3.10)

project(NinjaHSM)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Option to control test building
# Useful because if embedded projects use this as a library, they will
# likely encounter build errors if they try and pull in GoogleTest and/or
# build the test application.
option(NINJAHSM_BUILD_TESTS "Build NinjaHSM tests" OFF)

include_directories(include)
add_library(NinjaHSM INTERFACE)
target_include_directories(NinjaHSM INTERFACE include)

if(NINJAHSM_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(test)
endif()