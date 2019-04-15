project(remoting CXX)

include(define_targets)
include(show_target_properties)
include(platform)
include(dump_all_vars)

message("\n** Setting up ${PROJECT_NAME} **\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Remoting library")
set(PACKAGE_VERSION_MAJOR 21)
set(PACKAGE_VERSION_MINOR 000)
set(PACKAGE_VERSION_MICRO 4)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_COMPILER_DEFINITIONS_PRIVATE
    REMOTING_BUILD)

set(PACKAGE_COMPILER_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    REMOTING_STATIC)

set(PACKAGE_COMPILER_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    )

set(PACKAGE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/export
    )

set(PACKAGE_LINK_OPTIONS
    ${LINKER_OPTIONS}
    ${LINK_RPATH})

set(PACKAGE_DEPENDENCIES
    core
    )

if (MINGW)
    list(APPEND PACKAGE_COMPILER_OPTIONS
        -mno-ms-bitfields)
endif()
if (MINGW OR LINUX)
    list(APPEND PACKAGE_COMPILER_OPTIONS
        -Wno-missing-field-initializers)
endif()
if (WIN_MSVC OR MINGW)
    list(APPEND PACKAGE_DEPENDENCIES
#        Dbghelp.lib
#        Ws2_32.lib
#        Rpcrt4.dll
        IpHlpApi.lib)
elseif (LINUX OR APPLE)
#    list(APPEND PACKAGE_DEPENDENCIES
#        dl)
else()
    message(STATUS "Unsupported platform " ${CMAKE_HOST_SYSTEM})
    return()
endif()

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${LINKER_LIBRARIES}
    ${PACKAGE_DEPENDENCIES})

file(GLOB_RECURSE PACKAGE_SOURCES src/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_PUBLIC export/remoting/*.h)
file(GLOB_RECURSE PACKAGE_INCLUDES_PUBLIC_REMOTING export/remoting1/*.h)
file(GLOB_RECURSE PACKAGE_INCLUDES_PUBLIC_REMOTING_TS export/remotingts1/*.h)
file(GLOB_RECURSE PACKAGE_INCLUDES_PRIVATE include/remoting/*.h)

list(APPEND PACKAGE_INCLUDES_PUBLIC ${PACKAGE_INCLUDES_PUBLIC_REMOTING} ${PACKAGE_INCLUDES_PUBLIC_REMOTING_TS})
set(PACKAGE_BUILD_REFERENCE ${VERSION})

define_target_library_static()

show_target_properties(${PACKAGE_NAME})

if (NOT LOCAL_BUILD)
    add_install_target(${PACKAGE_NAME} install-components)
    add_uninstall_target(${PACKAGE_NAME} uninstall-components)
    install_pkgconfig_file(${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_NAME}.pc)

    install(TARGETS ${PACKAGE_NAME}
            EXPORT ${PACKAGE_NAME}
            DESTINATION ${PKGCONFIG_FILE_SHARED_LIB_DIR}
            COMPONENT ${PACKAGE_NAME})
endif()
