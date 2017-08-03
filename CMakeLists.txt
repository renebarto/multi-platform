cmake_minimum_required(VERSION 2.8)

project(multi-platform C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(config)
include(setup_standard_vars)
include(setup_default_options)

set(LANGUAGE CXX)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

find_package(Threads REQUIRED)

if (BUILD_MAJOR)
    set(VERSION_MAJOR ${BUILD_MAJOR})
else()
    set(VERSION_MAJOR 0)
endif()

if (BUILD_MINOR)
    set(VERSION_MINOR ${BUILD_MINOR})
else()
    set(VERSION_MINOR 0)
endif()

if (BUILD_REVISION)
    set(VERSION_REVISION ${BUILD_REVISION})
else()
    set(VERSION_REVISION 0)
endif()

if (BUILD_NUMBER)
    set(VERSION_BUILD ${BUILD_NUMBER})
else()
    set(VERSION_BUILD 0)
endif()

set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}.${VERSION_BUILD})

if(CMAKE_BUILD_TYPE)
    message(STATUS "Current build type: ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "Setting build to Debug")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CONFIG_DIR "debug" CACHE STRING "Build config directory" FORCE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CONFIG_DIR "release" CACHE STRING "Build config directory" FORCE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    set(CONFIG_DIR "releaseMinSize" CACHE STRING "Build config directory" FORCE)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(CONFIG_DIR "releaseMinSize" CACHE STRING "Build config directory" FORCE)
else()
    message(FATAL_ERROR "Invalid build type: " ${CMAKE_BUILD_TYPE})
endif()


set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

set(TEST_DATA_ROOT "${CMAKE_SOURCE_DIR}")
set(BUILD_TESTS ON)

message(STATUS "Build config directory  : " ${CMAKE_BINARY_DIR})
message(STATUS "Sytem root  directory   : " ${CMAKE_SYSROOT})
message(STATUS "Install prefix          : " ${CMAKE_INSTALL_PREFIX})
set(CMAKE_PROGRAM_PATH ${CMAKE_INSTALL_PREFIX}/bin)
message(STATUS "CMAKE_PROGRAM_PATH      : ${CMAKE_PROGRAM_PATH}")
set(CMAKE_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/lib)
message(STATUS "CMAKE_LIBRARY_PATH      : ${CMAKE_LIBRARY_PATH}")
set(CMAKE_INCLUDE_PATH ${CMAKE_INSTALL_PREFIX}/include)
message(STATUS "CMAKE_PROGRAM_PATH      : ${CMAKE_INCLUDE_PATH}")

set(OUTPUT_BASE_DIR ${CMAKE_SOURCE_DIR}/output)
message(STATUS "Output base directory   : " ${OUTPUT_BASE_DIR})

if (NOT WIN_MSVC AND NOT APPLE)
	set(LINK_WHOLE_LIB -Wl,--whole-archive)
	set(LINK_NO_WHOLE_LIB -Wl,--no-whole-archive)
	set(LINK_RPATH -Wl,-rpath,'\$ORIGIN/../lib/')
	set(CMAKE_SKIP_BUILD_RPATH TRUE)
endif()
if (APPLE)
	set(CMAKE_MACOSX_RPATH ../lib)
endif()

add_custom_target(install-components)
add_custom_target(uninstall-components)

setup_standard_vars()

setup_default_options()

# if (WEBBRIDGE_TEST_MEASUREMENTS)
    # set (GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
    # set (GCC_COVERAGE_LINK_FLAGS    "-lgcov")
    # set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    # set (CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
# endif (WEBBRIDGE_TEST_MEASUREMENTS)

add_subdirectory(source)
