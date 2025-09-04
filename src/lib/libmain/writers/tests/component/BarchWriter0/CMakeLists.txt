cmake_minimum_required(VERSION 3.13)

add_executable(
  CTEST_BarchWriter0
  CTEST_BarchWriter0.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/writers/BarchWriter0.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/converters/BMPAndBarchConverter0Base.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/images/BarchImage.cpp
)

target_include_directories(
  CTEST_BarchWriter0
  PRIVATE 
   ${GENERAL_MOCKS_ROOT}/log
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${CMAKE_SOURCE_DIR}/src/lib/facade/includes
)

target_link_libraries(
  CTEST_BarchWriter0
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_BarchWriter0
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)

