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

include(FetchContent)
FetchContent_Declare(
    etl
    GIT_REPOSITORY https://github.com/ETLCPP/etl.git
    GIT_TAG 20.39.4
)
FetchContent_MakeAvailable(etl)

add_library(NinjaHSM INTERFACE)
# Headers live under src/ (src/NinjaHSM/*.hpp) so the same layout works for CMake, PlatformIO
# and the Arduino IDE (which only compiles/exposes headers from a library's src/ directory).
target_include_directories(NinjaHSM INTERFACE src)
target_link_libraries(NinjaHSM INTERFACE etl::etl)

# Builds a minimal translation unit that instantiates the public API, without GoogleTest. Used by
# CI to verify the headers compile for embedded targets (cross-compiled ARM, exceptions/RTTI off).
# Built as a STATIC library so cross builds do not need a runtime/syscalls to link.
option(NINJAHSM_BUILD_COMPILE_CHECK "Build the headers-compile sanity check" OFF)
if(NINJAHSM_BUILD_COMPILE_CHECK)
    add_library(ninjahsm_compile_check STATIC test/compile_check.cpp)
    target_link_libraries(ninjahsm_compile_check PRIVATE NinjaHSM)
endif()

if(NINJAHSM_BUILD_TESTS)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(test)
endif()