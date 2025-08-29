cmake_minimum_required(VERSION 3.13)

add_executable(
  CTEST_BMPReader
  CTEST_BMPReader.cpp
  BMPReaderDataProvider_i1.cpp
  BMPReaderDataProvider_i2.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/readers/BMPReader.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/BMPImage.cpp
)

target_include_directories(
  CTEST_BMPReader
  PRIVATE ${GENERAL_MOCKS_ROOT}/LibraryContext
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
  PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/facade
)

target_link_libraries(
  CTEST_BMPReader
  GTest::gtest_main GTest::gmock
  TemplateProjectSimpleLoggerObj
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_BMPReader
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
