project(run-all-tests CXX)

include(setup_target_properties_executable)
include(show_target_properties)

message("\n** Setting up run-all-tests **\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "RUn all tests")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_COMPILER_DEFINITIONS_PRIVATE
    TEST_DATA_ROOT="${TEST_DATA_ROOT}"
    PLATFORM_NAME="${PLATFORM_NAME}")

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
    osal
    unit-test-c++
    crypto
    network
    xml
    json
    )

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${LINKER_LIBRARIES}
    ${PACKAGE_DEPENDENCIES})

file(GLOB PACKAGE_SOURCES src/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES include/*.h)

file(GLOB PACKAGE_SOURCES_OSAL ${CMAKE_SOURCE_DIR}/source/components/osal/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_OSAL ${CMAKE_SOURCE_DIR}/source/components/osal/test/include/osal/*.h)

file(GLOB PACKAGE_SOURCES_CORE ${CMAKE_SOURCE_DIR}/source/components/core/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_CORE ${CMAKE_SOURCE_DIR}/source/components/core/test/include/core/*.h)

file(GLOB PACKAGE_SOURCES_UNITTESTCPP ${CMAKE_SOURCE_DIR}/source/components/unit-test-c++/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_UNITTESTCPP ${CMAKE_SOURCE_DIR}/source/components/unit-test-c++/test/include/unit-test-c++/*.h)

file(GLOB PACKAGE_SOURCES_JSON ${CMAKE_SOURCE_DIR}/source/components/json/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_JSON ${CMAKE_SOURCE_DIR}/source/components/json/test/include/crypto/*.h)

file(GLOB PACKAGE_SOURCES_CRYPTO ${CMAKE_SOURCE_DIR}/source/components/crypto/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_CRYPTO ${CMAKE_SOURCE_DIR}/source/components/crypto/test/include/crypto/*.h)

file(GLOB PACKAGE_SOURCES_NETWORK ${CMAKE_SOURCE_DIR}/source/components/network/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_NETWORK ${CMAKE_SOURCE_DIR}/source/components/network/test/include/network/*.h)

file(GLOB PACKAGE_SOURCES_XML ${CMAKE_SOURCE_DIR}/source/components/xml/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_XML ${CMAKE_SOURCE_DIR}/source/components/xml/test/include/xml/*.h)

file(GLOB PACKAGE_SOURCES_JSON ${CMAKE_SOURCE_DIR}/source/components/json/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_JSON ${CMAKE_SOURCE_DIR}/source/components/json/test/include/xml/*.h)

list(APPEND PACKAGE_SOURCES
    ${PACKAGE_SOURCES_OSAL}
    ${PACKAGE_SOURCES_CORE}
    ${PACKAGE_SOURCES_UNITTESTCPP}
    ${PACKAGE_SOURCES_CRYPTO}
    ${PACKAGE_SOURCES_NETWORK}
    ${PACKAGE_SOURCES_XML}
    ${PACKAGE_SOURCES_JSON}
    )
list(APPEND PACKAGE_INCLUDES_PUBLIC
    ${PACKAGE_INCLUDES_OSAL}
    ${PACKAGE_INCLUDES_CORE}
    ${PACKAGE_INCLUDES_UNITTESTCPP}
    ${PACKAGE_INCLUDES_CRYPTO}
    ${PACKAGE_INCLUDES_NETWORK}
    ${PACKAGE_INCLUDES_XML}
    ${PACKAGE_INCLUDES_JSON}
    )
set(PACKAGE_INCLUDES_PUBLIC_PRIVATE
    )
list(APPEND PACKAGE_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/source/components/osal/test/include
    ${CMAKE_SOURCE_DIR}/source/components/core/test/include
    ${CMAKE_SOURCE_DIR}/source/components/unit-test-c++/test/include
    ${CMAKE_SOURCE_DIR}/source/components/crypto/test/include
    ${CMAKE_SOURCE_DIR}/source/components/network/test/include
    ${CMAKE_SOURCE_DIR}/source/components/xml/test/include
    ${CMAKE_SOURCE_DIR}/source/components/json/test/include
    )

set(PACKAGE_BUILD_REFERENCE ${VERSION})

define_target_executable()

show_target_properties(${PACKAGE_NAME})

if (NOT LOCAL_BUILD)
    add_install_target(${PACKAGE_NAME} install-components)
    add_uninstall_target(${PACKAGE_NAME} uninstall-components)

    install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION ${PKGCONFIG_FILE_BIN_DIR}
        COMPONENT ${PROJECT_NAME})
endif()
