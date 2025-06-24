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

# Add compiler flag -Wfatal-errors
target_compile_options(tests PRIVATE -Wfatal-errors)

include(GoogleTest)
gtest_discover_tests(tests)
