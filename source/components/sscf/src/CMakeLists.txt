project(sscf CXX)

include(define_targets)
include(show_target_properties)

message("\n** Setting up ${PROJECT_NAME} **\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "SSCF library")
set(PACKAGE_VERSION_MAJOR 21)
set(PACKAGE_VERSION_MINOR 000)
set(PACKAGE_VERSION_MICRO 4)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_COMPILER_DEFINITIONS_PRIVATE
    SSCF_BUILD)

set(PACKAGE_COMPILER_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    SSCF_STATIC)

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

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${LINKER_LIBRARIES}
    ${PACKAGE_DEPENDENCIES})

file(GLOB_RECURSE PACKAGE_SOURCES src/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_PUBLIC export/*.h)
file(GLOB_RECURSE PACKAGE_INCLUDES_PRIVATE include/*.h)

set(PACKAGE_BUILD_REFERENCE ${VERSION})

define_target_library_shared()

show_target_properties(${PACKAGE_NAME})

add_subdirectory(sscf)
add_subdirectory(sscfts)

setup_target_properties_library(${PROJECT_NAME})

# if (NOT LOCAL_BUILD)
#     add_uninstall_target(${PACKAGE_NAME} uninstall-components)
#     add_install_target(${PACKAGE_NAME} install-components)

#     install_pkgconfig_file(${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_NAME}.pc)

#     install(TARGETS ${PACKAGE_NAME}
#         ARCHIVE DESTINATION ${PKGCONFIG_FILE_LIB_DIR}
#         LIBRARY DESTINATION ${PKGCONFIG_FILE_LIB_DIR}
#         COMPONENT ${PACKAGE_NAME})
# endif()
