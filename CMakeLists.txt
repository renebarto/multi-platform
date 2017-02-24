cmake_minimum_required(VERSION 2.8)

project(multi-platform C CXX)

# add cmake custom module directory
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake CACHE STRING
    "CMake module path." FORCE)

include(config)
include(dump_vars)
include(setup_default_options)
include(platform)

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
set(BUILD_SHARED_LIBS ON)
set(BUILD_TESTS ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)
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
set(TEST_DATA_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

set(BUILD_BASE_DIR ${CMAKE_SOURCE_DIR}/build CACHE STRING
        "Build root directory.")
set(CMAKE_BINARY_DIR ${BUILD_BASE_DIR}/${CONFIG_DIR} CACHE STRING
    "Build directory.")
message(STATUS "Build base directory: " ${BUILD_BASE_DIR})
message(STATUS "Build config directory: " ${CMAKE_BINARY_DIR})

set(OUTPUT_BASE_DIR ${CMAKE_SOURCE_DIR}/output)
message(STATUS "Output base directory: " ${OUTPUT_BASE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/lib)
message(STATUS "Static lib output directory:  " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/bin)
message(STATUS "Dynamic lib output directory: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/bin)
message(STATUS "Executable output directory:  " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_MACOSX_RPATH ../lib)
set(CMAKE_SKIP_BUILD_RPATH TRUE)
message(STATUS "Install directory: " ${CMAKE_INSTALL_PREFIX})

# uninstall target
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake.in
    ${CMAKE_BINARY_DIR}/uninstall.cmake
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/uninstall.cmake)

# install target
# add_custom_target(install_${PROJECT_NAME}
    # make install
    # DEPENDS ${PROJECT_NAME}
    # COMMENT "Installing ${PROJECT_NAME}")

if(WIN_MSVC)
    # Important! do not keep warning 4251 4275 ignored!
    # Possibly switch warning 4505 back on
    set(FLAGS_CXX 
        /Wall /wd4251 /wd4275 /wd4505 /wd4514 /wd4548 /wd4571 /wd4625 /wd4626 /wd4710
        /wd4820 /wd5026 /wd5027 /WX- /EHsc /Gd /GR /sdl- /Zc:wchar_t /Zc:inline /fp:precise)
    set(FLAGS_CXX_DEBUG /Od /Gm- /ZI /RTC1 /MDd)
    set(FLAGS_CXX_RELEASE /Ox /GL /GS /Gy /Oi /MD)
    set(FLAGS_CXX_MINSIZEREL /O1 /GL /GS /Gy /Oi /MD)
    set(FLAGS_CXX_RELWITHDEBINFO /O2 /GL /GS /Gy /Oi /Zi /MD)

    set(FLAGS_C 
        /Wall /wd4251 /wd4275 /wd4505 /wd4514 /wd4548 /wd4571 /wd4625 /wd4626 /wd4710
        /wd4820 /wd5026 /wd5027 /WX- /EHsc /Gd /GR /sdl- /Zc:wchar_t /Zc:inline /fp:precise)
    set(FLAGS_C_DEBUG /Od /Gm- /ZI /RTC1 /MDd)
    set(FLAGS_C_RELEASE /Ox /GL /GS /Gy /Oi /MD)
    set(FLAGS_C_MINSIZEREL /O1 /GL /GS /Gy /Oi /MD)
    set(FLAGS_C_RELWITHDEBINFO /O2 /GL /GS /Gy /Oi /Zi /MD)

    set(DEFINES _UNICODE UNICODE _CRT_SECURE_NO_WARNINGS)
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

elseif(MINGW)
    set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC -fexceptions -fmessage-length=0)
    set(FLAGS_CXX_DEBUG -O0 -g)
    set(FLAGS_CXX_RELEASE -O3)
    set(FLAGS_CXX_MINSIZEREL -O3)
    set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

    set(FLAGS_C -Wall -Wextra -fPIC -fmessage-length=0)
    set(FLAGS_C_DEBUG -O0 -g)
    set(FLAGS_C_RELEASE -O3)
    set(FLAGS_C_MINSIZEREL -O3)
    set(FLAGS_C_RELWITHDEBINFO -O2 -g)

    set(DEFINES )
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

elseif(LINUX)
    set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC -fexceptions -fmessage-length=0)
    set(FLAGS_CXX_DEBUG -O0 -g)
    set(FLAGS_CXX_RELEASE -O3)
    set(FLAGS_CXX_MINSIZEREL -O3)
    set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

    set(FLAGS_C -Wall -Wextra -fPIC -fmessage-length=0)
    set(FLAGS_C_DEBUG -O0 -g)
    set(FLAGS_C_RELEASE -O3)
    set(FLAGS_C_MINSIZEREL -O3)
    set(FLAGS_C_RELWITHDEBINFO -O2 -g)

    set(DEFINES )
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

elseif(APPLE)
    set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC -fexceptions -fmessage-length=0)
    set(FLAGS_CXX_DEBUG -O0 -g)
    set(FLAGS_CXX_RELEASE -O3)
    set(FLAGS_CXX_MINSIZEREL -O3)
    set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

    set(FLAGS_C -Wall -Wextra -fPIC -fmessage-length=0)
    set(FLAGS_C_DEBUG -O0 -g)
    set(FLAGS_C_RELEASE -O3)
    set(FLAGS_C_MINSIZEREL -O3)
    set(FLAGS_C_RELWITHDEBINFO -O2 -g)

    set(DEFINES )
    set(DEFINES_DEBUG _DEBUG)
    set(DEFINES_RELEASE NDEBUG)
    set(DEFINES_MINSIZEREL NDEBUG)
    set(DEFINES_RELWITHDEBINFO NDEBUG)

else()
    message(STATUS "Unsupported platform " ${CMAKE_HOST_SYSTEM})
    return()
endif()

set(CMAKE_COMPILER_IS_CLANG OFF)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    message(STATUS "Using Clang")
    set(CMAKE_COMPILER_IS_CLANG ON)
elseif(${CMAKE_COMPILER_IS_GNUCXX})
    message(STATUS "Using Gcc")
else()
    message(STATUS "Compiler " ${CMAKE_CXX_COMPILER_ID})
endif()

if(CMAKE_USE_PTHREADS_INIT)
    message(STATUS "Adding pthread flags")
    list(APPEND FLAGS_CXX -pthread)
    list(APPEND FLAGS_C -pthread)
endif()

setup_default_options()

# if (WEBBRIDGE_TEST_MEASUREMENTS)
    # set (GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
    # set (GCC_COVERAGE_LINK_FLAGS    "-lgcov")
    # set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    # set (CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
# endif (WEBBRIDGE_TEST_MEASUREMENTS)

add_subdirectory(source)

# dump_vars()
