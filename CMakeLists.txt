cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("NosLib" VERSION 1.0.0 LANGUAGES CXX)

message(STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")

add_compile_definitions(CPPHTTPLIB_OPENSSL_SUPPORT)
add_compile_definitions(WIN32_LEAN_AND_MEAN)

option(HTTPLIB_REQUIRE_OPENSSL "Require SSL" ON)
option(HTTPLIB_USE_BROTLI_IF_AVAILABLE "" OFF)
option(HTTPLIB_USE_ZLIB_IF_AVAILABLE "" OFF)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

add_subdirectory(External)

add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE httplib::httplib)
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(${PROJECT_NAME} INTERFACE "./Include/")

if(PROJECT_IS_TOP_LEVEL)
	add_executable("NosLibTesting" "NosLibTesting/main.cpp")
	target_link_libraries("NosLibTesting" PRIVATE ${PROJECT_NAME})
endif()