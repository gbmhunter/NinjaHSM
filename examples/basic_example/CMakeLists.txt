cmake_minimum_required(VERSION 3.10)

project(basic_example)

include(FetchContent)
FetchContent_Declare(NinjaHSM
    GIT_REPOSITORY https://github.com/gbmhunter/NinjaHSM.git
    GIT_TAG 9574c886f30a3c5dc7f2628bf55a90cfbfb52b8d
    # GIT_TAG v0.1.0 # This can be a hash, tag or branch.
)
FetchContent_MakeAvailable(NinjaHSM)

add_executable(
    basic_example
    main.cpp
)
target_link_libraries(
  basic_example
  NinjaHSM
)
