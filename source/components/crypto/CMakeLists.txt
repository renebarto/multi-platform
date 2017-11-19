project(crypto CXX)

include(define_targets)
include(show_target_properties)

message("\n** Setting up crypto **\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Cryptographics library")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_COMPILER_DEFINITIONS_PRIVATE
    CRYPTO_BUILD)

set(PACKAGE_COMPILER_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    CRYPTO_DYNAMIC
    )

set(PACKAGE_COMPILER_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    )

set(PACKAGE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/export
    )

set(PACKAGE_LINK_OPTIONS
    ${LINKER_OPTIONS}
    ${LINK_RPATH} )

set(PACKAGE_DEPENDENCIES
    core)

if (WIN_MSVC OR MINGW)
    list(APPEND PACKAGE_DEPENDENCIES
        Dbghelp.lib)
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
    ${PACKAGE_DEPENDENCIES})

file(GLOB PACKAGE_SOURCES src/*.cpp)
file(GLOB PACKAGE_INCLUDES_PUBLIC export/crypto/*.h)
file(GLOB PACKAGE_INCLUDES_PRIVATE include/crypto/*.h)

set(PACKAGE_BUILD_REFERENCE ${VERSION})

define_target_library_shared()

show_target_properties(${PACKAGE_NAME})

if (NOT LOCAL_BUILD)
    add_uninstall_target(${PACKAGE_NAME} uninstall-components)
    add_install_target(${PACKAGE_NAME} install-components)

    install_pkgconfig_file(${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_NAME}.pc)

    install(TARGETS ${PACKAGE_NAME}
        ARCHIVE DESTINATION ${PKGCONFIG_FILE_LIB_DIR}
        LIBRARY DESTINATION ${PKGCONFIG_FILE_LIB_DIR}
        COMPONENT ${PACKAGE_NAME})
endif()
