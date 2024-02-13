cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

project ("NosLib")

message(STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE "./Include/")

add_executable("NosLibTesting" "NosLibTesting/main.cpp")
target_link_libraries("NosLibTesting" PUBLIC "NosLib")