cmake_minimum_required(VERSION 3.13)

add_executable(
  CTEST_LibMain
  CTEST_LibMain.cpp
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

#PRIVATE ${GENERAL_MOCKS_ROOT}/log
target_include_directories(
  CTEST_LibMain
  PRIVATE ${GENERAL_MOCKS_ROOT}/LibraryContext
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
  PRIVATE ${CMAKE_SOURCE_DIR}/src/lib/facade/includes
)

#TemplateProjectSimpleLoggerObj
target_link_libraries(
  CTEST_LibMain
  GTest::gtest_main GTest::gmock 
  TemplateProjectSimpleLoggerObj
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_LibMain
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
