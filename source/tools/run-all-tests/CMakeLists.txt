project(run-all-tests CXX)

include(setup_target_properties_executable)
include(show_target_properties)

message("\nrun-all-tests\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Run all tests")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(TOOLS_RUNALLTESTS_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    TEST_DATA_ROOT="${TEST_DATA_ROOT}")

set(TOOLS_RUNALLTESTS_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    )

set(TOOLS_RUNALLTESTS_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    )

set(TOOLS_RUNALLTESTS_LINK_OPTIONS
    ${LINK_RPATH})
list_to_string(TOOLS_RUNALLTESTS_LINK_OPTIONS TOOLS_RUNALLTESTS_LINK_OPTIONS_STRING)

set(TOOLS_RUNALLTESTS_DEPENDENCIES
    ${CMAKE_THREAD_LIBS_INIT}
    core
    osal
    unit-test-c++
    crypto
    network
    xml
    )

set(TOOLS_RUNALLTESTS_LIBS
    ${TOOLS_RUNALLTESTS_DEPENDENCIES})

file(GLOB TOOLS_RUNALLTESTS_SOURCES src/*.cpp)
file(GLOB_RECURSE TOOLS_RUNALLTESTS_INCLUDES include/*.h)

file(GLOB TOOLS_RUNALLTESTS_SOURCES_OSAL ${CMAKE_SOURCE_DIR}/source/components/osal/test/src/test/*.cpp)
file(GLOB_RECURSE TOOLS_RUNALLTESTS_INCLUDES_OSAL ${CMAKE_SOURCE_DIR}/source/components/osal/test/include/osal/*.h)

file(GLOB TOOLS_RUNALLTESTS_SOURCES_CORE ${CMAKE_SOURCE_DIR}/source/components/core/test/src/test/*.cpp)
file(GLOB_RECURSE TOOLS_RUNALLTESTS_INCLUDES_CORE ${CMAKE_SOURCE_DIR}/source/components/core/test/include/core/*.h)

file(GLOB TOOLS_RUNALLTESTS_SOURCES_UNITTESTCPP ${CMAKE_SOURCE_DIR}/source/components/unit-test-c++/test/src/test/*.cpp)
file(GLOB_RECURSE TOOLS_RUNALLTESTS_INCLUDES_UNITTESTCPP ${CMAKE_SOURCE_DIR}/source/components/unit-test-c++/test/include/unit-test-c++/*.h)

file(GLOB TOOLS_RUNALLTESTS_SOURCES_JSON ${CMAKE_SOURCE_DIR}/source/components/json/test/src/test/*.cpp)
file(GLOB_RECURSE TOOLS_RUNALLTESTS_INCLUDES_JSON ${CMAKE_SOURCE_DIR}/source/components/json/test/include/crypto/*.h)

file(GLOB TOOLS_RUNALLTESTS_SOURCES_CRYPTO ${CMAKE_SOURCE_DIR}/source/components/crypto/test/src/test/*.cpp)
file(GLOB_RECURSE TOOLS_RUNALLTESTS_INCLUDES_CRYPTO ${CMAKE_SOURCE_DIR}/source/components/crypto/test/include/crypto/*.h)

file(GLOB TOOLS_RUNALLTESTS_SOURCES_NETWORK ${CMAKE_SOURCE_DIR}/source/components/network/test/src/test/*.cpp)
file(GLOB_RECURSE TOOLS_RUNALLTESTS_INCLUDES_NETWORK ${CMAKE_SOURCE_DIR}/source/components/network/test/include/network/*.h)

file(GLOB TOOLS_RUNALLTESTS_SOURCES_XML ${CMAKE_SOURCE_DIR}/source/components/xml/test/src/test/*.cpp)
file(GLOB_RECURSE TOOLS_RUNALLTESTS_INCLUDES_XML ${CMAKE_SOURCE_DIR}/source/components/xml/test/include/xml/*.h)

list(APPEND TOOLS_RUNALLTESTS_SOURCES
    ${TOOLS_RUNALLTESTS_SOURCES_OSAL}
    ${TOOLS_RUNALLTESTS_SOURCES_CORE}
    ${TOOLS_RUNALLTESTS_SOURCES_UNITTESTCPP}
    ${TOOLS_RUNALLTESTS_SOURCES_CRYPTO}
    ${TOOLS_RUNALLTESTS_SOURCES_NETWORK}
    ${TOOLS_RUNALLTESTS_SOURCES_XML}
    )
list(APPEND TOOLS_RUNALLTESTS_INCLUDES
    ${TOOLS_RUNALLTESTS_INCLUDES_OSAL}
    ${TOOLS_RUNALLTESTS_INCLUDES_CORE}
    ${TOOLS_RUNALLTESTS_INCLUDES_UNITTESTCPP}
    ${TOOLS_RUNALLTESTS_INCLUDES_JSON}
    ${TOOLS_RUNALLTESTS_INCLUDES_CRYPTO}
    ${TOOLS_RUNALLTESTS_INCLUDES_NETWORK}
    ${TOOLS_RUNALLTESTS_INCLUDES_XML}
    )
list(APPEND TOOLS_RUNALLTESTS_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/source/components/osal/test/include
    ${CMAKE_SOURCE_DIR}/source/components/core/test/include
    ${CMAKE_SOURCE_DIR}/source/components/unit-test-c++/test/include
    ${CMAKE_SOURCE_DIR}/source/components/crypto/test/include
    ${CMAKE_SOURCE_DIR}/source/components/json/test/include
    ${CMAKE_SOURCE_DIR}/source/components/network/test/include
    ${CMAKE_SOURCE_DIR}/source/components/xml/test/include
    )

display_list("Defines                     : " ${TOOLS_RUNALLTESTS_DEFINITIONS} )
display_list("Compiler options            : " ${TOOLS_RUNALLTESTS_OPTIONS} )
display_list("Source files                : " ${TOOLS_RUNALLTESTS_SOURCES} )
display_list("Source files                : " ${TOOLS_RUNALLTESTS_SOURCES_OSAL} )
display_list("Source files                : " ${TOOLS_RUNALLTESTS_SOURCES_CORE} )
display_list("Source files                : " ${TOOLS_RUNALLTESTS_SOURCES_UNITTESTCPP} )
display_list("Source files                : " ${TOOLS_RUNALLTESTS_SOURCES_CRYPTO} )
display_list("Source files                : " ${TOOLS_RUNALLTESTS_SOURCES_NETWORK} )
display_list("Source files                : " ${TOOLS_RUNALLTESTS_SOURCES_XML} )
display_list("Include files               : " ${TOOLS_RUNALLTESTS_INCLUDES} )
display_list("Include dirs                : " ${TOOLS_RUNALLTESTS_INCLUDE_DIRS} )
display_list("Link libs                   : " ${TOOLS_RUNALLTESTS_LIBS} )
display_list("Linker options              : " ${TOOLS_RUNALLTESTS_LINK_OPTIONS_STRING} )
display_list("Dependencies                : " ${TOOLS_RUNALLTESTS_DEPENDENCIES} )

# add the executable
add_executable(${PROJECT_NAME} ${TOOLS_RUNALLTESTS_SOURCES} ${TOOLS_RUNALLTESTS_INCLUDES})
target_link_libraries(${PROJECT_NAME} ${TOOLS_RUNALLTESTS_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${TOOLS_RUNALLTESTS_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${TOOLS_RUNALLTESTS_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PRIVATE ${TOOLS_RUNALLTESTS_OPTIONS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${TOOLS_RUNALLTESTS_LINK_OPTIONS_STRING}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PACKAGE_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME run-all-tests)

setup_target_properties_executable(${PROJECT_NAME})

show_target_properties(${PROJECT_NAME})

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
    COMPONENT network)
