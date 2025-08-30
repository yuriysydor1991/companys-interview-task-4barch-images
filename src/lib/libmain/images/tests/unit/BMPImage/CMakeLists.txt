cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_BMPImage
  UTEST_BMPImage.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/images/BMPImage.cpp
)

target_include_directories(
  UTEST_BMPImage
  PRIVATE 
   ${GENERAL_MOCKS_ROOT}/log
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${CMAKE_SOURCE_DIR}/src/lib/facade
)

target_link_libraries(
  UTEST_BMPImage
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_BMPImage
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)

