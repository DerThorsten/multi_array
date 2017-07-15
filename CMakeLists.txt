cmake_minimum_required(VERSION 3.1)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

project(nifty)

set (MULTI_ARRAY_VERSION_MAJOR 0 )
set (MULTI_ARRAY_VERSION_MINOR 1 )
set (MULTI_ARRAY_VERSION_PATCH 0 )


set (MULTI_ARRAY_VERSION_SHORT_STR "${MULTI_ARRAY_VERSION_MAJOR}.${MULTI_ARRAY_VERSION_MINOR}")
set (MULTI_ARRAY_VERSION_STR "${MULTI_ARRAY_VERSION_MAJOR}.${MULTI_ARRAY_VERSION_MINOR}.${MULTI_ARRAY_VERSION_PATCH}")


include(CheckCXXCompilerFlag)


SET(BUILD_TESTS ON CACHE BOOL "")
option(BUILD_TESTS    ON)


SET(BUILD_BENCHMARK ON CACHE BOOL "")
option(BUILD_BENCHMARK    ON)


#-------------------------------------------------------------------------------------------------------------------
# check for c++ 11 / 14 support
#-------------------------------------------------------------------------------------------------------------------

# FIXME c++14 checks are broken!
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)
    if (HAS_CPP14_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    elseif (HAS_CPP11_FLAG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
        message(FATAL_ERROR "Unsupported compiler -- multi-array  requires C++11 support!")
    endif()
endif()


#-------------------------------------------------------------------------------------------------------------------
# include include files
#-------------------------------------------------------------------------------------------------------------------
file(GLOB_RECURSE headers include/*.hxx)
include_directories(include)


message(STATUS "OSEfsr")
#############################
# external projects
#############################
add_subdirectory(external_projects)



# this needs to be enabled here
if(BUILD_TESTS)
    enable_testing()
endif()


#-------------------------------------------------------------------------------------------------------------------
# src folder
#-------------------------------------------------------------------------------------------------------------------
add_subdirectory(src)

