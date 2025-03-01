cmake_minimum_required (VERSION 3.12)

include("CMake/Versioning.cmake")

Get_Git_Version()
Get_Git_Branch()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project ("NosLib" VERSION 1.0.0 LANGUAGES CXX)

message(STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(HTTPLIB_REQUIRE_OPENSSL "Require SSL" ON)
option(HTTPLIB_USE_BROTLI_IF_AVAILABLE "" OFF)
option(HTTPLIB_USE_ZLIB_IF_AVAILABLE "" OFF)

add_compile_definitions(CPPHTTPLIB_OPENSSL_SUPPORT)
add_compile_definitions(WIN32_LEAN_AND_MEAN)

add_subdirectory(External)
add_subdirectory(NosLib)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_subdirectory(NosLibTesting)
endif()