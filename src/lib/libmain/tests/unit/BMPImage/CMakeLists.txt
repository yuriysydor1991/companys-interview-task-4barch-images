cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_BMPImage
  UTEST_BMPImage.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/BMPImage.cpp
)

target_include_directories(
  UTEST_BMPImage
  PRIVATE ${GENERAL_MOCKS_ROOT}/LibraryContext
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
  PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/facade
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
