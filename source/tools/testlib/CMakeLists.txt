project(testlib CXX)

include(setup_target_properties_library)
include(show_target_properties)

message("\n** Setting up ${PROJECT_NAME} **\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Shared library for testing")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_COMPILER_DEFINITIONS_PRIVATE
    )

set(PACKAGE_COMPILER_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    )

set(PACKAGE_COMPILER_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    )

set(PACKAGE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    )

set(PACKAGE_LINK_OPTIONS
    ${LINKER_OPTIONS}
    ${LINK_RPATH})

set(PACKAGE_DEPENDENCIES
    osal)

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${LINKER_LIBRARIES}
    ${PACKAGE_DEPENDENCIES})

file(GLOB PACKAGE_SOURCES src/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_PUBLIC include/*.h)

set(PACKAGE_INCLUDES_PUBLIC_PRIVATE
    )

set(PACKAGE_BUILD_REFERENCE ${VERSION})

define_target_library_shared()

show_target_properties(${PACKAGE_NAME})

if (NOT LOCAL_BUILD)
    add_install_target(${PACKAGE_NAME} install-components)
    add_uninstall_target(${PACKAGE_NAME} uninstall-components)

    install(TARGETS ${PROJECT_NAME}
        LIBRARY
        DESTINATION ${PKGCONFIG_FILE_BIN_DIR}
        COMPONENT ${PROJECT_NAME})
endif()
