cmake_minimum_required(VERSION 3.13)

target_sources(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE 
    BMP2BarchConverter0.cpp
    BMPAndBarchConverter0Base.cpp
)

add_subdirectory(tests)

