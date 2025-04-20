project ("NosLib" VERSION ${GIT_VERSION} LANGUAGES CXX)

file(GLOB_RECURSE LibrarySourceFiles CONFIGURE_DEPENDS "Source/*")

if(WIN32)
	set(Boost_USE_STATIC_LIBS OFF)
	set(Boost_USE_STATIC_RUNTIME OFF)
	set(Boost_USE_MULTITHREADED ON)
endif()

set(BoostComponents serialization filesystem)

if(NOSLIB_REQUIRE_BOOST)
	find_package(Boost REQUIRED COMPONENTS ${BoostComponents})
else()
	find_package(Boost COMPONENTS ${BoostComponents})
endif()

if(NOSLIB_REQUIRE_OPENSSL)
	find_package(OpenSSL REQUIRED)
else()
	find_package(OpenSSL)
endif()

if(NOSLIB_REQUIRE_CPPHTTPLIB)
	find_package(httplib CONFIG REQUIRED)
else()
	find_package(httplib CONFIG)
endif()

set(NOSLIB_INSTALL_INCLUDEDIR "Include")

add_library(${PROJECT_NAME} ${LibrarySourceFiles})
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include> $<INSTALL_INTERFACE:${NOSLIB_INSTALL_INCLUDEDIR}>)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

Set_Harsh_Compiler_flags(${PROJECT_NAME})

if(Boost_FOUND)
	message(STATUS "Using Boost")
	target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost Boost::serialization Boost::filesystem)
	target_compile_definitions(${PROJECT_NAME} PUBLIC NOSLIB_USE_BOOST)
endif()

if(OpenSSL_FOUND)
	message(STATUS "Using OpenSSL")
	target_link_libraries(${PROJECT_NAME} PUBLIC OpenSSL::Crypto OpenSSL::SSL)
	target_compile_definitions(${PROJECT_NAME} PUBLIC NOSLIB_USE_OPENSSL)
endif()

if(httplib_FOUND)
	message(STATUS "Using HttpLib")
	target_link_libraries(${PROJECT_NAME} PUBLIC httplib::httplib)
	target_compile_definitions(${PROJECT_NAME} PUBLIC NOSLIB_USE_HTTPLIB)
endif()

if(BUILD_SHARED_LIBS)
	target_compile_definitions(${PROJECT_NAME} PRIVATE NOSLIB_EXPORT)
	target_compile_definitions(${PROJECT_NAME} PUBLIC NOSLIB_SHARED)
endif()

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

include(GNUInstallDirs)

# --- Installation and Package Export ---
# 1. Install the library target
install(TARGETS ${PROJECT_NAME}
	EXPORT NosLibTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/$<CONFIG>    # For static libraries (e.g., .lib files)
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/$<CONFIG>    # For shared libraries (e.g., .dll/.so files)
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/$<CONFIG>    # For Windows DLLs (the executable part)
	PUBLIC_HEADER DESTINATION ${NOSLIB_INSTALL_INCLUDEDIR}
)

# 2. Install public headers
install(DIRECTORY Include/ DESTINATION ${NOSLIB_INSTALL_INCLUDEDIR})

# 3. Export the targets to a file so that consumers can import them with find_package()
install(EXPORT NosLibTargets
	FILE NosLibTargets.cmake
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# 4. Create package configuration files
include(CMakePackageConfigHelpers)

# Configure the package config file using an input template
configure_package_config_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/CMake/${PROJECT_NAME}Config.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Generate a version file for the package
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# 5. Install the package configuration files
install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)