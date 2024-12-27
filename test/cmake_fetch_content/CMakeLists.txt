include(FetchContent)
FetchContent_Declare(NinjaHSM
    GIT_REPOSITORY https://github.com/gbmhunter/NinjaHSM.git
    GIT_TAG 1784bd67ea3905e21b1c7c654afbba0fcab2249d
)
FetchContent_MakeAvailable(NinjaHSM)

add_executable(
    cmake_fetch_content
    main.cpp
)
target_link_libraries(
  cmake_fetch_content
  NinjaHsm
)
