cmake_minimum_required(VERSION 3.5.1)

project(multi-platform C CXX)

option(MEASURE_COVERAGE "Determine coverage" OFF)
option(BUILD_UNIT_TESTS "Build unit tests" ON)
option(LOCAL_BUILD "Only perform local build, no install" OFF)
option(SCRIPTS_DIR "CMake scripts location" "${CMAKE_SOURCE_DIR}/cmake")

message(STATUS "Scripts location: ${SCRIPTS_DIR}")
list(APPEND CMAKE_MODULE_PATH ${SCRIPTS_DIR})

include(define_targets)
include(functions)
include(setup_pkgconfig_file_vars)
include(setup_default_options)

set(LANGUAGE CXX)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

find_package(Threads REQUIRED)

setup_version()

message(STATUS "Version: ${VERSION}")

set_build_type()
set_config_dir()

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

set(OUTPUT_BASE_DIR ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_PROGRAM_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(CMAKE_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INCLUDE_PATH ${CMAKE_INSTALL_PREFIX}/include)

message(STATUS "Build config directory  : ${CMAKE_BINARY_DIR}")
message(STATUS "Sytem root  directory   : ${CMAKE_SYSROOT}")
message(STATUS "Install prefix          : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_PROGRAM_PATH      : ${CMAKE_PROGRAM_PATH}")
message(STATUS "CMAKE_LIBRARY_PATH      : ${CMAKE_LIBRARY_PATH}")
message(STATUS "CMAKE_PROGRAM_PATH      : ${CMAKE_INCLUDE_PATH}")
message(STATUS "Output base directory   : ${OUTPUT_BASE_DIR}")

setup_library_definitions()

if (NOT LOCAL_BUILD)
    add_custom_target(install-components)
    add_custom_target(uninstall-components)
endif()

setup_pkgconfig_file_vars()

setup_default_options()

if ("${PLATFORM_NAME}" STREQUAL "")
    set(PLATFORM_NAME "x86_64")
endif()

if ("${TEST_DATA_ROOT}" STREQUAL "")
    if (LOCAL_BUILD)
        set(TEST_DATA_ROOT "${CMAKE_SOURCE_DIR}/testdata")
    else()
        set(TEST_DATA_ROOT "/usr/share/testdata")
    endif()
endif()

add_subdirectory(source)
if (BUILD_UNIT_TESTS)
    if (NOT LOCAL_BUILD)
        add_install_target(testdata install-components)
        add_uninstall_target(testdata uninstall-components)
        message(STATUS "Install test data to " ${TEST_DATA_ROOT})
        install(DIRECTORY testdata
            DESTINATION ${TEST_DATA_ROOT}/..
            COMPONENT testdata)
    endif()
endif()
