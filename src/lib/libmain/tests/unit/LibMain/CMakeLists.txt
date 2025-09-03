cmake_minimum_required(VERSION 3.13)

# @todo: replace all real code with mocks!
add_executable(
  UTEST_LibMain
  UTEST_LibMain.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/LibMain.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/converters/BMP2BarchConverter0.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/converters/BMPAndBarchConverter0Base.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/converters/Barch2BMPConverter0.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/readers/BMPReader.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/readers/BarchReader0.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/writers/BarchWriter0.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/images/BMPImage.cpp
  ${CMAKE_SOURCE_DIR}/src/lib/libmain/images/BarchImage.cpp
)

target_include_directories(
  UTEST_LibMain
  PRIVATE ${GENERAL_MOCKS_ROOT}/LibraryContext
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
  PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/facade
)

target_link_libraries(
  UTEST_LibMain
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_LibMain
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
