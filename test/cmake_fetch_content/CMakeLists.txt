cmake_minimum_required(VERSION 3.10)

project(cmake_fetch_content)

include(FetchContent)
FetchContent_Declare(NinjaHSM
    GIT_REPOSITORY https://github.com/gbmhunter/NinjaHSM.git
    GIT_TAG v0.1.0 # This can be a hash, tag or branch.
)
FetchContent_MakeAvailable(NinjaHSM)

# Add a message to confirm that NinjaHSM is available
message(STATUS "NinjaHSM has been fetched and is available.")

add_executable(
    cmake_fetch_content
    main.cpp
)
target_link_libraries(
  cmake_fetch_content
  PRIVATE
  NinjaHsm
)

get_target_property(NINJAHSM_INCLUDE_DIRS NinjaHsm INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "NinjaHsm include directories: ${NINJAHSM_INCLUDE_DIRS}")