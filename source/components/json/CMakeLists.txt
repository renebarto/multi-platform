project(json CXX)

include(setup_target_properties_library)
include(show_target_properties)
include(display_list)

message("\njson\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "OS abstraction layer")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(COMPONENTS_JSON_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    JSON_DYNAMIC)

set(COMPONENTS_JSON_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    )

set(COMPONENTS_JSON_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/export
    )

set(COMPONENTS_JSON_LINK_OPTIONS
    ${LINKER_OPTIONS}
    ${LINK_RPATH})
list_to_string(COMPONENTS_JSON_LINK_OPTIONS COMPONENTS_JSON_LINK_OPTIONS_STRING)

set(COMPONENTS_JSON_DEPENDENCIES
    core
    )

set(COMPONENTS_JSON_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${LINKER_LIBRARIES}
    ${COMPONENTS_JSON_DEPENDENCIES})

file(GLOB_RECURSE COMPONENTS_JSON_SOURCES src/*.cpp)
file(GLOB_RECURSE COMPONENTS_JSON_INCLUDES_EXPORT export/*.h)
file(GLOB_RECURSE COMPONENTS_JSON_INCLUDES include/*.h)

set(COMPONENTS_JSON_INCLUDES
    ${COMPONENTS_JSON_INCLUDES_EXPORT}
    ${COMPONENTS_JSON_INCLUDES})

display_list("Defines                     : " ${COMPONENTS_JSON_DEFINITIONS} )
display_list("Compiler options            : " ${COMPONENTS_JSON_OPTIONS} )
display_list("Source files                : " ${COMPONENTS_JSON_SOURCES} )
display_list("Include files               : " ${COMPONENTS_JSON_INCLUDES} )
display_list("Include dirs                : " ${COMPONENTS_JSON_INCLUDE_DIRS} )
display_list("Link libs                   : " ${COMPONENTS_JSON_LIBS} )
display_list("Linker options              : " ${COMPONENTS_JSON_LINK_OPTIONS} )
display_list("Dependencies                : " ${COMPONENTS_JSON_DEPENDENCIES} )

# add the library
add_library(${PROJECT_NAME} SHARED ${COMPONENTS_JSON_SOURCES} ${COMPONENTS_JSON_INCLUDES})
target_link_libraries(${PROJECT_NAME} ${COMPONENTS_JSON_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${COMPONENTS_JSON_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE UNITTESTCPP_BUILD)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${COMPONENTS_JSON_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPONENTS_JSON_OPTIONS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${COMPONENTS_JSON_LINK_OPTIONS_STRING}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PACKAGE_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME json)
setup_target_properties_library(${PROJECT_NAME})

show_target_properties(${PROJECT_NAME})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/json.pc.in
    ${CMAKE_CURRENT_SOURCE_DIR}/json.pc
    @ONLY)

# uninstall target
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/uninstall.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
    IMMEDIATE @ONLY)

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
    ARCHIVE DESTINATION ${TOOLS_LIB_DIR}
    LIBRARY DESTINATION ${TOOLS_LIB_DIR}
    COMPONENT ${PROJECT_NAME})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/json.pc
    DESTINATION ${TOOLS_PKG_CONFIG_DIR}
    COMPONENT ${PROJECT_NAME})
