cmake_minimum_required(VERSION 3.13)

add_library(
  ${PROJECT_LIBRARY_NAME}
  SHARED
)

target_include_directories(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
  INTERFACE facade
)

set_target_properties(
  ${PROJECT_LIBRARY_NAME}
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(
  TARGETS ${PROJECT_LIBRARY_NAME}
  LIBRARY
    DESTINATION lib
    COMPONENT Runtime
    NAMELINK_COMPONENT Development
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ${PROJECT_LIBRARY_NAME}
)

add_subdirectory(facade)
add_subdirectory(libmain)
