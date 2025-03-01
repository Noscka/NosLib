project ("NosLib" VERSION ${GIT_VERSION} LANGUAGES CXX)

file(GLOB_RECURSE LibrarySourceFiles CONFIGURE_DEPENDS "Source/*")

add_library(${PROJECT_NAME} ${LibrarySourceFiles})
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(${PROJECT_NAME} INTERFACE httplib::httplib)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

target_compile_definitions(${PROJECT_NAME} PRIVATE NOSLIB_EXPORTS)
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WIN32_LEAN_AND_MEAN)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    MAP_IMPORTED_CONFIG_DEBUG "Debug"
    MAP_IMPORTED_CONFIG_RELEASE "Release"
    MAP_IMPORTED_CONFIG_RELWITHDEBINFO "Release"
    MAP_IMPORTED_CONFIG_MINSIZEREL "Release"
)

if(WIN32 AND BUILD_SHARED_LIBS)
  set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# --- Installation and Package Export ---
# 1. Install the library target
install(TARGETS ${PROJECT_NAME}
    EXPORT NosLibTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/$<CONFIG>    # For static libraries (e.g., .lib files)
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/$<CONFIG>    # For shared libraries (e.g., .dll/.so files)
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/$<CONFIG>    # For Windows DLLs (the executable part)
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 2. Install public headers
install(DIRECTORY Include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 3. Export the targets to a file so that consumers can import them with find_package()
install(EXPORT NosLibTargets
    FILE NosLibTargets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# 4. Create package configuration files
include(CMakePackageConfigHelpers)

# Generate a version file for the package
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Configure the package config file using an input template
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# 5. Install the package configuration files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)