cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_Barch2BMPConverter0
  UTEST_Barch2BMPConverter0.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/converters/Barch2BMPConverter0.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/converters/BMPAndBarchConverter0Base.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/images/BarchImage.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/images/BMPImage.cpp
)

#${GENERAL_MOCKS_ROOT}/log
target_include_directories(
  UTEST_Barch2BMPConverter0
  PRIVATE 
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${CMAKE_SOURCE_DIR}/src/lib/facade
)

target_link_libraries(
  UTEST_Barch2BMPConverter0
  GTest::gtest_main GTest::gmock
  TemplateProjectSimpleLoggerObj
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_Barch2BMPConverter0
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)

