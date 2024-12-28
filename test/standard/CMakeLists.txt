enable_testing()

add_executable(
  hello_test
  tests.cpp
)
target_link_libraries(
  hello_test
  NinjaHSM
  GTest::gtest_main
  gmock_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)