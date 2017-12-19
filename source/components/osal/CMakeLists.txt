project(osal CXX)

include(define_targets)
include(show_target_properties)

message("\n** Setting up ${PROJECT_NAME} **\n")

find_package(libuuid-dev REQUIRED)

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "OS abstraction layer")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_COMPILER_DEFINITIONS_PRIVATE
    OSAL_BUILD)

set(PACKAGE_COMPILER_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    OSAL_DYNAMIC
    )

set(PACKAGE_COMPILER_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    )

set(PACKAGE_INCLUDE_DIRS
    ${LIBUUID_DEV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/export
    )

set(PACKAGE_LINK_OPTIONS
    ${LINKER_OPTIONS}
    ${LINK_RPATH}
    )

set(PACKAGE_DEPENDENCIES
    )

if (WIN_MSVC OR MINGW)
    list(APPEND PACKAGE_DEPENDENCIES
        Dbghelp.lib
        Ws2_32.lib)
elseif (LINUX OR APPLE)
    list(APPEND PACKAGE_DEPENDENCIES
        )
else()
    message(STATUS "Unsupported platform " ${CMAKE_HOST_SYSTEM})
    return()
endif()

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${LINKER_LIBRARIES}
    ${LIBUUID_DEV_LIBRARIES}
    ${PACKAGE_DEPENDENCIES})

file(GLOB PACKAGE_SOURCES_BASE src/*.cpp)
file(GLOB PACKAGE_INCLUDES_PUBLIC_BASE export/osal/*.h)
file(GLOB PACKAGE_INCLUDES_PRIVATE_BASE include/osal/*.h)

if (WIN_MSVC)
    message(STATUS "Building on Windows with MSVC")
    file(GLOB PACKAGE_SOURCES_PLATFORM src/windows/*.cpp)
    file(GLOB PACKAGE_INCLUDES_PUBLIC_PLATFORM export/osal/windows/*.h)
    file(GLOB PACKAGE_INCLUDES_PRIVATE_PLATFORM include/osal/windows/*.h)
elseif(MINGW)
    message(STATUS "Building on Windows with MinGW")
    file(GLOB PACKAGE_SOURCES_PLATFORM src/mingw/*.cpp)
    file(GLOB PACKAGE_INCLUDES_PUBLIC_PLATFORM export/osal/mingw/*.h)
    file(GLOB PACKAGE_INCLUDES_PRIVATE_PLATFORM include/osal/mingw/*.h)
elseif(LINUX)
    message(STATUS "Building on Linux")
    file(GLOB PACKAGE_SOURCES_PLATFORM src/linux/*.cpp)
    file(GLOB PACKAGE_INCLUDES_PUBLIC_PLATFORM export/osal/linux/*.h)
    file(GLOB PACKAGE_INCLUDES_PRIVATE_PLATFORM include/osal/linux/*.h)
elseif(APPLE)
    message(STATUS "Building on OS X")
    file(GLOB PACKAGE_SOURCES_PLATFORM src/darwin/*.cpp)
    file(GLOB PACKAGE_INCLUDES_PUBLIC_PLATFORM export/osal/darwin/*.h)
    file(GLOB PACKAGE_INCLUDES_PRIVATE_PLATFORM include/osal/darwin/*.h)
else()
    message(STATUS "Unsupported platform " ${CMAKE_HOST_SYSTEM})
    _return()
endif()

set(PACKAGE_SOURCES
    ${PACKAGE_SOURCES_BASE}
    ${PACKAGE_SOURCES_PLATFORM})
set(PACKAGE_INCLUDES_PUBLIC
    ${PACKAGE_INCLUDES_PUBLIC_BASE}
    ${PACKAGE_INCLUDES_PUBLIC_PLATFORM})
set(PACKAGE_INCLUDES_PRIVATE
    ${PACKAGE_INCLUDES_PRIVATE_BASE}
    ${PACKAGE_INCLUDES_PRIVATE_PLATFORM})

set(PACKAGE_BUILD_REFERENCE ${VERSION})

define_target_library_shared()

show_target_properties(${PACKAGE_NAME})

if (NOT LOCAL_BUILD)
    add_install_target(${PACKAGE_NAME} install-components)
    add_uninstall_target(${PACKAGE_NAME} uninstall-components)
    install_pkgconfig_file(${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_NAME}.pc)

    install(TARGETS ${PACKAGE_NAME}
        ARCHIVE DESTINATION ${PKGCONFIG_FILE_LIB_DIR}
        LIBRARY DESTINATION ${PKGCONFIG_FILE_LIB_DIR}
        COMPONENT ${PACKAGE_NAME})
endif()


