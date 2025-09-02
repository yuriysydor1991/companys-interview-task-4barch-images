cmake_minimum_required(VERSION 3.13)

add_executable(
  CTEST_BarchReader0
  CTEST_BarchReader0.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/readers/BarchReader0.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/images/BarchImage.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/converters/BMPAndBarchConverter0Base.cpp
)

#  PRIVATE ${GENERAL_MOCKS_ROOT}/log
target_include_directories(
  CTEST_BarchReader0
  PRIVATE ${GENERAL_MOCKS_ROOT}/LibraryContext
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
  PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/facade
)

target_link_libraries(
  CTEST_BarchReader0
  GTest::gtest_main GTest::gmock
  TemplateProjectSimpleLoggerObj
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_BarchReader0
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
