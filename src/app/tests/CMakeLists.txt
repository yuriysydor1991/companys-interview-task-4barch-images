cmake_minimum_required(VERSION 3.13)

if(NOT ENABLE_UNIT_TESTS)
  return()
endif()

add_compile_options(-DNDEBUG=1)

set(APP_MOCKS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/mocks")

add_subdirectory(unit)
