cmake_minimum_required (VERSION 3.12)

find_package(NosCMLib REQUIRED)

Get_Git_Version()
Get_Git_Branch()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project ("NosLib" VERSION 1.0.0 LANGUAGES CXX)

message(STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(NOSLIB_REQUIRE_OPENSSL "Require OpenSSL" ON)
option(NOSLIB_REQUIRE_BOOST "Require Boost" ON)

set(HTTPLIB_REQUIRE_OPENSSL ${NOSLIB_REQUIRE_OPENSSL} CACHE BOOL "Require OpenSSL (overridden)" FORCE)
add_subdirectory(External)
target_compile_definitions(httplib INTERFACE CPPHTTPLIB_OPENSSL_SUPPORT)
add_subdirectory(NosLib)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_subdirectory(NosLibTesting)
endif()