cmake_minimum_required(VERSION 3.10)

project(basic_example)

# Required for the VSCode C/C++ extension to work
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build against the version of NinjaHSM in this repo at this point in time. This is what CI uses
# so that the example always exercises the in-repo source (rather than a published release that
# may not yet contain features used by main.cpp).
add_subdirectory(../../ ${CMAKE_CURRENT_BINARY_DIR}/ninjahsm)

# If you instead want to see how a downstream project pulls in a released version of NinjaHSM,
# comment out the add_subdirectory() line above and use the FetchContent block below.
# include(FetchContent)
# FetchContent_Declare(NinjaHSM
#     GIT_REPOSITORY https://github.com/gbmhunter/NinjaHSM.git
#     GIT_TAG v1.2.0 # This can be a hash, tag or branch.
# )
# FetchContent_MakeAvailable(NinjaHSM)

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
