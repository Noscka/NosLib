function(Get_Git_Version)
	execute_process(COMMAND git describe --tags
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_RESULT
                    OUTPUT_VARIABLE GIT_VERSION)

    if(NOT GIT_RESULT EQUAL 0)
        message(WARNING "Git describe failed, using v0.0.0 instead.")
        set(GIT_VERSION "v0.0.0")
    endif()

    message(STATUS "CMAKE_CXX_STANDARD = ${GIT_VERSION}")

    string(REPLACE "\n" "" GIT_VERSION ${GIT_VERSION})
    string(REPLACE " " "" GIT_VERSION ${GIT_VERSION})

    if( GIT_VERSION MATCHES "^v(0|[1-9][0-9]*)[.](0|[1-9][0-9]*)[.](0|[1-9][0-9]*)(.*)$" )
        set(version_major "${CMAKE_MATCH_1}")
        set(version_minor "${CMAKE_MATCH_2}")
        set(version_patch "${CMAKE_MATCH_3}")
    else()
        message(WARNING "Wrong Git Tag Format: [${GIT_VERSION}]")
    endif()

    message(STATUS "Git Tag: ${version_major}.${version_minor}.${version_patch}")

    set(GIT_VERSION "${version_major}.${version_minor}.${version_patch}" PARENT_SCOPE)
endfunction()

function(Get_Git_Branch)
    execute_process(COMMAND git rev-parse --abbrev-ref HEAD
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_RESULT
                    OUTPUT_VARIABLE GIT_BRANCH)

     if(NOT GIT_RESULT EQUAL 0)
        message(WARNING "Git rev-parse failed, using DEFAULT instead.")
        set(GIT_BRANCH "DEFAULT")
    endif()

    string(REPLACE "\n" "" GIT_BRANCH ${GIT_BRANCH})
    string(REPLACE " " "" GIT_BRANCH ${GIT_BRANCH})
    
    message(STATUS "Git Branch: ${GIT_BRANCH}")

    set(GIT_BRANCH "${GIT_BRANCH}" PARENT_SCOPE)
endfunction()