# - Try to find libuuid-dev
# Once done this will define
#  LIBUUID_DEV_FOUND - System has libuuid-dev
#  LIBUUID_DEV_INCLUDE_DIRS - The libuuid-dev include directories
#  LIBUUID_DEV_LIBRARIES - The libraries needed to use libuuid-dev
#  LIBUUID_DEV_LIBRARY_DRS - The libraries directories needed to use libuuid-dev
#

# This is to ensure that if CMAKE_INSTALL_PREFIX is set to a non-default location, pkgconfig can find the .pc file
if(NOT CMAKE_CROSSCOMPILING)
    set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
    set(CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX})
    if (CMAKE_VERBOSE_MAKEFILE)
        message(STATUS "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
    endif()
endif()

find_package(PkgConfig)
pkg_check_modules(PC_LIBUUID_DEV uuid)

if(PC_LIBUUID_DEV_FOUND)
    if(LIBUUID_DEV_FIND_VERSION AND PC_LIBUUID_DEV_VERSION)
        if ("${LIBUUID_DEV_FIND_VERSION}" VERSION_GREATER "${PC_LIBUUID_DEV_VERSION}")
            message(WARNING "Incorrect version, found ${PC_LIBUUID_DEV_VERSION}, need at least ${LIBUUID_DEV_FIND_VERSION}, please install correct version ${LIBUUID_DEV_FIND_VERSION}")
            set(LIBUUID_DEV_FOUND_TEXT "Found incorrect version")
            unset(PC_LIBUUID_DEV_FOUND)
        endif()
    endif()
else()
    set(LIBUUID_DEV_FOUND_TEXT "Not found")
endif()

if(PC_LIBUUID_DEV_FOUND)
    find_path(
        LIBUUID_DEV_INCLUDE_DIRS
        NAMES uuid/uuid.h
        HINTS ${PC_LIBUUID_DEV_INCLUDE_DIRS} ${PC_LIBUUID_DEV_INCLUDEDIR})
    
    set(LIBRARIES uuid)
    set(LIBUUID_DEV_LIBRARIES )
    foreach(LIB ${LIBRARIES})
        find_library(LIBUUID_DEV_LIBRARY_${LIB} NAMES ${LIB}
            HINTS ${PC_LIBUUID_DEV_LIBRARY_DIRS} ${PC_LIBUUID_DEV_LIBDIR})
        list(APPEND LIBUUID_DEV_LIBRARIES ${LIBUUID_DEV_LIBRARY_${LIB}})
    endforeach()
    
    if("${LIBUUID_DEV_INCLUDE_DIRS}" STREQUAL "" OR "${LIBUUID_DEV_LIBRARIES}" STREQUAL "")
        set(LIBUUID_DEV_FOUND_TEXT "Not found")
    else()
        set(LIBUUID_DEV_FOUND_TEXT "Found")
    endif()
else()
    set(LIBUUID_DEV_FOUND_TEXT "Not found")
endif()

if (CMAKE_VERBOSE_MAKEFILE)
    message(STATUS "libuuid        : ${LIBUUID_DEV_FOUND_TEXT}")
    message(STATUS "  version      : ${PC_LIBUUID_DEV_VERSION}")
    message(STATUS "  cflags       : ${PC_LIBUUID_DEV_CFLAGS}")
    message(STATUS "  cflags other : ${PC_LIBUUID_DEV_CFLAGS_OTHER}")
    message(STATUS "  include dirs : ${PC_LIBUUID_DEV_INCLUDE_DIRS} ${PC_LIBUUID_DEV_INCLUDEDIR}")
    message(STATUS "  lib dirs     : ${PC_LIBUUID_DEV_LIBRARY_DIRS} ${PC_LIBUUID_DEV_LIBDIR}")
    message(STATUS "  include dirs : ${LIBUUID_DEV_INCLUDE_DIRS}")
    message(STATUS "  libs         : ${LIBUUID_DEV_LIBRARIES}")
endif()

set(LIBUUID_DEV_INCLUDE_DIR ${LIBUUID_DEV_INCLUDE_DIRS})
set(LIBUUID_DEV_LIBRARY ${LIBUUID_DEV_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBUUID_DEV DEFAULT_MSG
    LIBUUID_DEV_LIBRARIES LIBUUID_DEV_INCLUDE_DIRS)

if(LIBUUID_DEV_FOUND)
else()
    message(WARNING "Could not find libuuid-dev, please install")
endif()

mark_as_advanced(LIBUUID_DEV_DEFINITIONS LIBUUID_DEV_INCLUDE_DIRS LIBUUID_DEV_LIBRARIES)
