cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)

project ("NosLib")

if(WIN32)
message(STATUS "Using /MT flag on windows")
add_compile_options (/MT)
elseif(UNIX)
message(STATUS "Using -static flag on linux")
add_compile_options ("-static")
add_link_options("-static")
endif()


message(STATUS "CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}")

option(BUILD_SHARED_LIBS "If should link dynamically or statically" OFF)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE "./Include/")