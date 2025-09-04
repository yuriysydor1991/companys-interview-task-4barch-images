cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_BarchImage
  UTEST_BarchImage.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/images/BarchImage.cpp
)

target_include_directories(
  UTEST_BarchImage
  PRIVATE 
   ${GENERAL_MOCKS_ROOT}/log
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${CMAKE_SOURCE_DIR}/src/lib/facade/includes
)

target_link_libraries(
  UTEST_BarchImage
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_BarchImage
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)

