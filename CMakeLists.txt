cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("NosLib" VERSION 1.0.0 LANGUAGES CXX)

message(STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(PROJECT_IS_TOP_LEVEL)
	add_executable("NosLibTesting" "NosLibTesting/main.cpp")
	target_link_libraries("NosLibTesting" PRIVATE ${PROJECT_NAME})
endif()