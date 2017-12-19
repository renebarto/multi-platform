project(crypto.test CXX)

include(setup_target_properties_executable)
include(show_target_properties)

message("\n** Setting up ${PROJECT_NAME} **\n")

find_package(UnitTest-C++ REQUIRED)

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Crypto test application")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_COMPILER_DEFINITIONS_PRIVATE
    )

set(PACKAGE_COMPILER_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    TEST_DATA_ROOT="${TEST_DATA_ROOT}")

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
    core
    crypto)

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIB_CLANG_LIB}
    ${UNITTEST_CPP_LIBRARIES}
    ${LINKER_LIBRARIES}
    ${PACKAGE_DEPENDENCIES})

file(GLOB PACKAGE_SOURCES src/*.cpp)
file(GLOB PACKAGE_SOURCES_TEST src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_PUBLIC include/*.h)
set(PACKAGE_INCLUDES_PUBLIC_PRIVATE
    )
list(APPEND PACKAGE_SOURCES ${PACKAGE_SOURCES_TEST})

set(PACKAGE_BUILD_REFERENCE ${VERSION})

define_target_executable()

show_target_properties(${PACKAGE_NAME})

if (NOT LOCAL_BUILD)
    add_custom_target(uninstall_${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -DCOMPONENT=${PROJECT_NAME} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
        DEPENDS ${PROJECT_NAME}
        COMMENT "Uninstalling ${PROJECT_NAME}")

    add_custom_target(install_${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -DCOMPONENT=${PROJECT_NAME} -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
        DEPENDS ${PROJECT_NAME}
        COMMENT "Installing ${PROJECT_NAME}")

    add_dependencies(install-components install_${PROJECT_NAME})

    add_dependencies(uninstall-components uninstall_${PROJECT_NAME})

    install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        COMPONENT crypto)
endif()
