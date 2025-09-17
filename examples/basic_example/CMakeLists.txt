cmake_minimum_required(VERSION 3.10)

project(basic_example)

# Required for the VSCode C/C++ extension to work
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(NinjaHSM
    GIT_REPOSITORY https://github.com/gbmhunter/NinjaHSM.git
    GIT_TAG v1.2.0 # This can be a hash, tag or branch.
)
FetchContent_MakeAvailable(NinjaHSM)

# Comment out the FetchContent stuff above and use this below if you want to run the example on the version of NinjaHSM in the repo at this point in time. Useful for testing.
# add_subdirectory(../../ ${CMAKE_CURRENT_BINARY_DIR}/ninjahsm)

add_executable(
    basic_example
    main.cpp
)
target_link_libraries(
  basic_example
  PRIVATE
  NinjaHSM
)

# Set compiler flag to fail on fatal error
target_compile_options(basic_example PRIVATE -Wfatal-errors)
