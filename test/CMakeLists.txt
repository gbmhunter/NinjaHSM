enable_testing()

add_executable(
  tests
  tests.cpp
)
target_link_libraries(
  tests
  NinjaHSM
  GTest::gtest_main
  gmock_main
)

include(GoogleTest)
gtest_discover_tests(tests)
