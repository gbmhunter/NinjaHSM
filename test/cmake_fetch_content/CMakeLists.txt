cmake_minimum_required(VERSION 3.10)

project(cmake_fetch_content)

include(FetchContent)
FetchContent_Declare(NinjaHSM
    GIT_REPOSITORY https://github.com/gbmhunter/NinjaHSM.git
    GIT_TAG b1c00d11cbc0ab7a8a68ce8c4804759a430ea2be
    # GIT_TAG v0.1.0 # This can be a hash, tag or branch.
)
FetchContent_MakeAvailable(NinjaHSM)

add_executable(
    cmake_fetch_content
    main.cpp
)
target_link_libraries(
  cmake_fetch_content
  NinjaHSM
)

get_target_property(NINJAHSM_INCLUDE_DIRS NinjaHSM INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "NinjaHsm include directories: ${NINJAHSM_INCLUDE_DIRS}")