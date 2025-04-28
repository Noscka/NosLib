cmake_minimum_required (VERSION 3.5)

add_subdirectory(NosCMLib)

Get_Git_Version()
Get_Git_Branch()

#get_catch2()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project ("NosLib" LANGUAGES CXX)

message(STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(NOSLIB_REQUIRE_OPENSSL "Require OpenSSL" ON)
option(NOSLIB_REQUIRE_BOOST "Require Boost" ON)
option(NOSLIB_REQUIRE_CPPHTTPLIB "Require Cpp-HttpLib" ON)

#add_subdirectory(External)

add_subdirectory(NosLib)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	add_subdirectory(NosLibExperimenting)
endif()