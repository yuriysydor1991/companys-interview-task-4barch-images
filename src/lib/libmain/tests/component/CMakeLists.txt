cmake_minimum_required(VERSION 3.13)

if (NOT ENABLE_COMPONENT_TESTS)
  return()
endif()

# disabling the asserts for the tests
add_compile_options(-DNDEBUG=1)

configure_file(LibMain_includes.h.in LibMain_includes.h)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}  
)

add_subdirectory(LibMain)

