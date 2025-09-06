include(GNUInstallDirs)

install(
  TARGETS ${PROJECT_LIBRARY_NAME}
  EXPORT ${PROJECT_LIBRARY_NAME}
  LIBRARY
    DESTINATION lib
    COMPONENT Runtime
    NAMELINK_COMPONENT Development
)

# Export targets (so downstream can use find_package)
install(EXPORT ${PROJECT_LIBRARY_NAME}
  FILE ${PROJECT_LIBRARY_NAME}Targets.cmake
  NAMESPACE ${PROJECT_LIBRARY_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_LIBRARY_NAME}
)

# Generate a ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}${PROJECT_LIBRARY_NAME}Version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

# Generate a Config.cmake
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_LIBRARY_NAME}Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_LIBRARY_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_LIBRARY_NAME}
)

install(EXPORT ${PROJECT_LIBRARY_NAME}
  FILE "${PROJECT_LIBRARY_NAME}Config.cmake"
  NAMESPACE ${PROJECT_LIBRARY_NAME}::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_LIBRARY_NAME}
)

