project(regex)

include(define_targets)
include(show_target_properties)
include(platform)
include(dump_all_vars)

message("\n** Setting up ${PROJECT_NAME} **\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Regular expression library")
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
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/export
    )

set(PACKAGE_LINK_OPTIONS
    ${LINKER_OPTIONS}
    ${LINK_RPATH}
    )

set(PACKAGE_DEPENDENCIES
    osal)

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
#    ${LINKER_LIBRARIES}
    ${PACKAGE_DEPENDENCIES})

file(GLOB PACKAGE_SOURCES src/*.cpp)
file(GLOB PACKAGE_INCLUDES_PUBLIC export/regex/*.h)
file(GLOB PACKAGE_INCLUDES_PRIVATE include/regex/*.h)

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


