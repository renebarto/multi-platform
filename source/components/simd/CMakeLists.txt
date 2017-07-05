project(simd CXX)

include(setup_target_properties_library)
include(show_target_properties)
include(display_list)

message("\nsimd\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "OS abstraction layer")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(COMPONENTS_SIMD_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    SIMD_DYNAMIC
    )

set(COMPONENTS_SIMD_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    )

set(COMPONENTS_SIMD_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/export
    )

set(COMPONENTS_SIMD_LINK_OPTIONS
    ${LINK_RPATH} )

list_to_string(COMPONENTS_SIMD_LINK_OPTIONS COMPONENTS_SIMD_LINK_OPTIONS_STRING)

set(COMPONENTS_SIMD_DEPENDENCIES
    )

if (WIN_MSVC OR MINGW)
    list(APPEND COMPONENTS_SIMD_DEPENDENCIES
        Dbghelp.lib)
elseif (LINUX OR APPLE)
    list(APPEND COMPONENTS_SIMD_DEPENDENCIES
        )
else()
    message(STATUS "Unsupported platform " ${CMAKE_HOST_SYSTEM})
    return()
endif()

set(COMPONENTS_SIMD_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${COMPONENTS_SIMD_DEPENDENCIES})

file(GLOB COMPONENTS_SIMD_SOURCES_BASE src/*.cpp)
file(GLOB COMPONENTS_SIMD_INCLUDES_EXPORT_BASE export/simd/*.h)
file(GLOB COMPONENTS_SIMD_INCLUDES_BASE include/simd/*.h)

if (WIN_MSVC)
    message(STATUS "Building on Windows with MSVC")
    file(GLOB COMPONENTS_SIMD_SOURCES src/windows/*.cpp)
    file(GLOB COMPONENTS_SIMD_INCLUDES_EXPORT export/simd/windows/*.h)
    file(GLOB COMPONENTS_SIMD_INCLUDES include/simd/windows/*.h)
elseif(MINGW)
    message(STATUS "Building on Windows with MinGW")
    file(GLOB COMPONENTS_SIMD_SOURCES src/mingw/*.cpp)
    file(GLOB COMPONENTS_SIMD_INCLUDES_EXPORT export/simd/mingw/*.h)
    file(GLOB COMPONENTS_SIMD_INCLUDES include/simd/mingw/*.h)
elseif(LINUX)
    message(STATUS "Building on Linux")
    file(GLOB COMPONENTS_SIMD_SOURCES src/linux/*.cpp)
    file(GLOB COMPONENTS_SIMD_INCLUDES_EXPORT export/simd/linux/*.h)
    file(GLOB COMPONENTS_SIMD_INCLUDES include/simd/linux/*.h)
elseif(APPLE)
    message(STATUS "Building on OS X")
    file(GLOB COMPONENTS_SIMD_SOURCES src/darwin/*.cpp)
    file(GLOB COMPONENTS_SIMD_INCLUDES_EXPORT export/simd/darwin/*.h)
    file(GLOB COMPONENTS_SIMD_INCLUDES include/simd/darwin/*.h)
else()
    message(STATUS "Unsupported platform " ${CMAKE_HOST_SYSTEM})
    _return()
endif()

set(COMPONENTS_SIMD_SOURCES
    ${COMPONENTS_SIMD_SOURCES_BASE}
    ${COMPONENTS_SIMD_SOURCES})
set(COMPONENTS_SIMD_INCLUDES
    ${COMPONENTS_SIMD_INCLUDES_EXPORT_BASE}
    ${COMPONENTS_SIMD_INCLUDES_EXPORT}
    ${COMPONENTS_SIMD_INCLUDES_BASE}
    ${COMPONENTS_SIMD_INCLUDES})

display_list("Defines                     : " ${COMPONENTS_SIMD_DEFINITIONS} )
display_list("Compiler options            : " ${COMPONENTS_SIMD_OPTIONS} )
display_list("Source files                : " ${COMPONENTS_SIMD_SOURCES} )
display_list("Include files               : " ${COMPONENTS_SIMD_INCLUDES} )
display_list("Include dirs                : " ${COMPONENTS_SIMD_INCLUDE_DIRS} )
display_list("Link libs                   : " ${COMPONENTS_SIMD_LIBS} )
display_list("Linker options              : " ${COMPONENTS_SIMD_LINK_OPTIONS} )
display_list("Dependencies                : " ${COMPONENTS_SIMD_DEPENDENCIES} )

# add the library
add_library(${PROJECT_NAME} SHARED ${COMPONENTS_SIMD_SOURCES} ${COMPONENTS_SIMD_INCLUDES})
target_link_libraries(${PROJECT_NAME} ${COMPONENTS_SIMD_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${COMPONENTS_SIMD_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE SIMD_BUILD)
target_compile_definitions(${PROJECT_NAME} PUBLIC ${COMPONENTS_SIMD_DEFINITIONS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPONENTS_SIMD_OPTIONS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${COMPONENTS_SIMD_LINK_OPTIONS_STRING}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PACKAGE_VERSION_MAJOR})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME simd)
setup_target_properties_library(${PROJECT_NAME})

show_target_properties(${PROJECT_NAME})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/simd.pc.in
    ${CMAKE_CURRENT_SOURCE_DIR}/simd.pc
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
    LIBRARY
    DESTINATION ${TOOLS_LIB_DIR}
    COMPONENT ${PROJECT_NAME})

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/simd.pc
    DESTINATION ${TOOLS_PKG_CONFIG_DIR}
    COMPONENT ${PROJECT_NAME})


