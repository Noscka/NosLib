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

InstallProject(${PROJECT_NAME})