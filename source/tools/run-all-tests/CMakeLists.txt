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
    ${LINK_WHOLE_LIB}
    core.test-lib
    crypto.test-lib
    network.test-lib
    osal.test-lib
    simd.test-lib
    unit-test-c++.test-lib
    ${LINK_NO_WHOLE_LIB})

set(TOOLS_RUNALLTESTS_LIBS
    ${TOOLS_RUNALLTESTS_DEPENDENCIES})

file(GLOB TOOLS_RUNALLTESTS_SOURCES src/*.cpp)
file(GLOB_RECURSE TOOLS_RUNALLTESTS_INCLUDES include/*.h)

display_list("Defines                     : " ${TOOLS_RUNALLTESTS_DEFINITIONS} )
display_list("Compiler options            : " ${TOOLS_RUNALLTESTS_OPTIONS} )
display_list("Source files                : " ${TOOLS_RUNALLTESTS_SOURCES} )
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
