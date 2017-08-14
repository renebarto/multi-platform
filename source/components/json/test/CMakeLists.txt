project(json.test CXX)

include(setup_target_properties_executable)
include(show_target_properties)

message("\njson.test\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Unit test framework test application")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(COMPONENTS_JSON_TEST_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    TEST_DATA_ROOT="${TEST_DATA_ROOT}")

set(COMPONENTS_JSON_TEST_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    )

set(COMPONENTS_JSON_TEST_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    )

set(COMPONENTS_JSON_TEST_LINK_OPTIONS
    ${LINK_RPATH}
    )
list_to_string(COMPONENTS_JSON_TEST_LINK_OPTIONS COMPONENTS_JSON_TEST_LINK_OPTIONS_STRING)

set(COMPONENTS_JSON_TEST_DEPENDENCIES
    ${CMAKE_THREAD_LIBS_INIT}
    osal
    core
    json
    unit-test-c++)

set(COMPONENTS_JSON_TEST_LIBS
    ${COMPONENTS_JSON_TEST_DEPENDENCIES})

file(GLOB COMPONENTS_JSON_TEST_SOURCES src/*.cpp)
file(GLOB COMPONENTS_JSON_TEST_SOURCES_TEST src/test/*.cpp)
file(GLOB_RECURSE COMPONENTS_JSON_TEST_INCLUDES include/*.h)

display_list("Defines                     : " ${COMPONENTS_JSON_TEST_DEFINITIONS} )
display_list("Compiler options            : " ${COMPONENTS_JSON_TEST_OPTIONS} )
display_list("Source files                : " ${COMPONENTS_JSON_TEST_SOURCES} )
display_list("Include files               : " ${COMPONENTS_JSON_TEST_INCLUDES} )
display_list("Include dirs                : " ${COMPONENTS_JSON_TEST_INCLUDE_DIRS} )
display_list("Link libs                   : " ${COMPONENTS_JSON_TEST_LIBS} )
display_list("Linker options              : " ${COMPONENTS_JSON_TEST_LINK_OPTIONS_STRING} )
display_list("Dependencies                : " ${COMPONENTS_JSON_TEST_DEPENDENCIES} )

# add the executable
add_executable(${PROJECT_NAME} ${COMPONENTS_JSON_TEST_SOURCES} ${COMPONENTS_JSON_TEST_SOURCES_TEST} ${COMPONENTS_JSON_TEST_INCLUDES})
target_link_libraries(${PROJECT_NAME} ${COMPONENTS_JSON_TEST_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${COMPONENTS_JSON_TEST_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${COMPONENTS_JSON_TEST_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPONENTS_JSON_TEST_OPTIONS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${COMPONENTS_JSON_TEST_LINK_OPTIONS_STRING}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PACKAGE_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME json.test)

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
    COMPONENT core)