cmake_minimum_required(VERSION 3.13)

target_sources(
  ${PROJECT_LIBRARY_NAME}
  PRIVATE 
    LibMain.cpp
    LibFactory.cpp
    BarchImage.cpp
    BMPImage.cpp
)

add_subdirectory(tests)
add_subdirectory(readers)
