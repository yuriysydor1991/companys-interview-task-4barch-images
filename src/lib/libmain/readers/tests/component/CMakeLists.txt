cmake_minimum_required(VERSION 3.13)

if(NOT ENABLE_COMPONENT_TESTS)
  return()
endif()

configure_file(readers_includes.h.in readers_includes.h)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(BMPReader)

