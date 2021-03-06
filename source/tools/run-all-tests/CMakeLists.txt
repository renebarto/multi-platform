project(run-all-tests CXX)

include(setup_target_properties_executable)
include(show_target_properties)

message("\n** Setting up ${PROJECT_NAME} **\n")

if(LINUX OR DARWIN)
    find_package(unittest-cpp REQUIRED)
else()
    include(${PLATFORM_INSTALL}/unittest-cpp.cmake)
endif()

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Run all tests")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_COMPILER_DEFINITIONS_PRIVATE
    TEST_DATA_ROOT="${TEST_DATA_ROOT}"
    PLATFORM_NAME="${PLATFORM_NAME}"
    MYSQLPP_MYSQL_HEADERS_BURIED)

set(PACKAGE_COMPILER_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    TEST_DATA_ROOT="${TEST_DATA_ROOT}")

set(PACKAGE_COMPILER_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    )

set(PACKAGE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    )
if(LINUX OR DARWIN)
set(PACKAGE_INCLUDE_DIRS
    ${PACKAGE_INCLUDE_DIRS}
    ${UNITTEST_CPP_INCLUDE_DIRS})
endif()

set(PACKAGE_LINK_OPTIONS
    ${LINKER_OPTIONS}
    ${LINK_RPATH})

set(PACKAGE_DEPENDENCIES
    core
    osal
    network
    json
    settings
    remoting
#    regex
#    parser
#    database
#    crypto
#    xml
#    parse-c++
#    staruml
    )
if(WIN_MSVC OR MINGW)
set(PACKAGE_DEPENDENCIES
    ${PACKAGE_DEPENDENCIES}
    unittest-cpp)
endif()

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIB_CLANG_LIB}
    ${LINKER_LIBRARIES}
    ${PACKAGE_DEPENDENCIES})
if(LINUX OR DARWIN)
set(PACKAGE_LIBS
    ${PACKAGE_LIBS}
    ${UNITTEST_CPP_LIBRARIES})
endif()

file(GLOB_RECURSE PACKAGE_SOURCES src/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_PUBLIC include/*.h)

file(GLOB_RECURSE PACKAGE_SOURCES_OSAL ${CMAKE_SOURCE_DIR}/source/components/osal/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_OSAL ${CMAKE_SOURCE_DIR}/source/components/osal/test/include/osal/*.h)

file(GLOB_RECURSE PACKAGE_SOURCES_CORE ${CMAKE_SOURCE_DIR}/source/components/core/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_CORE ${CMAKE_SOURCE_DIR}/source/components/core/test/include/core/*.h)

file(GLOB_RECURSE PACKAGE_SOURCES_NETWORK ${CMAKE_SOURCE_DIR}/source/components/network/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_NETWORK ${CMAKE_SOURCE_DIR}/source/components/network/test/include/network/*.h)

file(GLOB_RECURSE PACKAGE_SOURCES_JSON ${CMAKE_SOURCE_DIR}/source/components/json/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_JSON ${CMAKE_SOURCE_DIR}/source/components/json/test/include/json/*.h)

file(GLOB_RECURSE PACKAGE_SOURCES_SETTINGS ${CMAKE_SOURCE_DIR}/source/components/settings/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_SETTINGS ${CMAKE_SOURCE_DIR}/source/components/settings/test/include/settings/*.h)

file(GLOB_RECURSE PACKAGE_SOURCES_REMOTING ${CMAKE_SOURCE_DIR}/source/components/remoting/test/src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_REMOTING ${CMAKE_SOURCE_DIR}/source/components/remoting/test/include/remoting/*.h)

#file(GLOB_RECURSE PACKAGE_SOURCES_RE ${CMAKE_SOURCE_DIR}/source/components/regex/test/src/test/*.cpp)
#file(GLOB_RECURSE PACKAGE_INCLUDES_RE ${CMAKE_SOURCE_DIR}/source/components/regex/test/include/regex/*.h)
#
#file(GLOB_RECURSE PACKAGE_SOURCES_PARSER ${CMAKE_SOURCE_DIR}/source/components/parser/test/src/test/*.cpp)
#file(GLOB_RECURSE PACKAGE_INCLUDES_PARSER ${CMAKE_SOURCE_DIR}/source/components/parser/test/include/parser/*.h)
#
#file(GLOB_RECURSE PACKAGE_SOURCES_DATABASE ${CMAKE_SOURCE_DIR}/source/components/database/test/src/test/*.cpp)
#file(GLOB_RECURSE PACKAGE_INCLUDES_DATABASE ${CMAKE_SOURCE_DIR}/source/components/database/test/include/crypto/*.h)
#
#file(GLOB_RECURSE PACKAGE_SOURCES_CRYPTO ${CMAKE_SOURCE_DIR}/source/components/crypto/test/src/test/*.cpp)
#file(GLOB_RECURSE PACKAGE_INCLUDES_CRYPTO ${CMAKE_SOURCE_DIR}/source/components/crypto/test/include/crypto/*.h)
#
#file(GLOB_RECURSE PACKAGE_SOURCES_XML ${CMAKE_SOURCE_DIR}/source/components/xml/test/src/test/*.cpp)
#file(GLOB_RECURSE PACKAGE_INCLUDES_XML ${CMAKE_SOURCE_DIR}/source/components/xml/test/include/xml/*.h)
#
#file(GLOB_RECURSE PACKAGE_SOURCES_JSON ${CMAKE_SOURCE_DIR}/source/components/json/test/src/test/*.cpp)
#file(GLOB_RECURSE PACKAGE_INCLUDES_JSON ${CMAKE_SOURCE_DIR}/source/components/json/test/include/xml/*.h)
#
#file(GLOB_RECURSE PACKAGE_SOURCES_PARSE_CPP ${CMAKE_SOURCE_DIR}/source/components/parse-c++/test/src/test/*.cpp)
#file(GLOB_RECURSE PACKAGE_INCLUDES_PARSE_CPP ${CMAKE_SOURCE_DIR}/source/components/parse-c++/test/include/parse-c++/*.h)
#
#file(GLOB_RECURSE PACKAGE_SOURCES_STARUML ${CMAKE_SOURCE_DIR}/source/components/staruml/test/src/test/*.cpp)
#file(GLOB_RECURSE PACKAGE_INCLUDES_STARUML ${CMAKE_SOURCE_DIR}/source/components/staruml/test/include/xml/*.h)

list(APPEND PACKAGE_SOURCES
    ${PACKAGE_SOURCES_OSAL}
    ${PACKAGE_SOURCES_CORE}
    ${PACKAGE_SOURCES_NETWORK}
    ${PACKAGE_SOURCES_JSON}
    ${PACKAGE_SOURCES_SETTINGS}
    ${PACKAGE_SOURCES_REMOTING}
#    ${PACKAGE_SOURCES_RE}
#    ${PACKAGE_SOURCES_PARSER}
#    ${PACKAGE_SOURCES_DATABASE}
#    ${PACKAGE_SOURCES_CRYPTO}
#    ${PACKAGE_SOURCES_XML}
#    ${PACKAGE_SOURCES_PARSE_CPP}
#    ${PACKAGE_SOURCES_STARUML}
    )
list(APPEND PACKAGE_INCLUDES_PUBLIC
    ${PACKAGE_INCLUDES_OSAL}
    ${PACKAGE_INCLUDES_CORE}
    ${PACKAGE_INCLUDES_NETWORK}
    ${PACKAGE_INCLUDES_JSON}
    ${PACKAGE_INCLUDES_SETTINGS}
    ${PACKAGE_INCLUDES_REMOTING}
#    ${PACKAGE_INCLUDES_RE}
#    ${PACKAGE_INCLUDES_PARSER}
#    ${PACKAGE_INCLUDES_DATABASE}
#    ${PACKAGE_INCLUDES_CRYPTO}
#    ${PACKAGE_INCLUDES_XML}
#    ${PACKAGE_INCLUDES_PARSE_CPP}
#    ${PACKAGE_INCLUDES_STARUML}
    )
set(PACKAGE_INCLUDES_PUBLIC_PRIVATE
    )
list(APPEND PACKAGE_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/source/components/osal/test/include
    ${CMAKE_SOURCE_DIR}/source/components/core/test/include
    ${CMAKE_SOURCE_DIR}/source/components/network/test/include
    ${CMAKE_SOURCE_DIR}/source/components/json/test/include
    ${CMAKE_SOURCE_DIR}/source/components/settings/test/include
    ${CMAKE_SOURCE_DIR}/source/components/remoting/test/include
#    ${CMAKE_SOURCE_DIR}/source/components/regex/test/include
#    ${CMAKE_SOURCE_DIR}/source/components/parser/test/include
#    ${CMAKE_SOURCE_DIR}/source/components/database/test/include
#    ${CMAKE_SOURCE_DIR}/source/components/crypto/test/include
#    ${CMAKE_SOURCE_DIR}/source/components/xml/test/include
#    ${CMAKE_SOURCE_DIR}/source/components/parse-c++/test/include
#    ${CMAKE_SOURCE_DIR}/source/components/staruml/test/include
    )

set(PACKAGE_BUILD_REFERENCE ${VERSION})

define_target_executable()

add_dependencies(${PACKAGE_NAME} echotest testlib)

show_target_properties(${PACKAGE_NAME})

if (NOT LOCAL_BUILD)
    add_install_target(${PACKAGE_NAME} install-components)
    add_uninstall_target(${PACKAGE_NAME} uninstall-components)

    install(TARGETS ${PROJECT_NAME}
        RUNTIME
        DESTINATION ${PKGCONFIG_FILE_BIN_DIR}
        COMPONENT ${PROJECT_NAME})
endif()
