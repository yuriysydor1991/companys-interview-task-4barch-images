cmake_minimum_required(VERSION 3.13)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(
  Qt6 REQUIRED 
  COMPONENTS Core
)

qt_add_library(
  TheBarchCoderQt6ModelsObj OBJECT
  MANUAL_FINALIZATION
  FileListModel.cpp
)

target_include_directories(
  TheBarchCoderQt6ModelsObj
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE Qt6::Core
)

target_link_libraries(
  TheBarchCoderQt6ModelsObj
  Qt6::Core
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  TheBarchCoderQt6ModelsObj
)

qt_finalize_target(TheBarchCoderQt6ModelsObj)

