include(display_list)

function(setup_target_properties_library target)
    if ("${OUTPUT_BASE_DIR}" STREQUAL "")
        message(ERROR "Invalid: OUTPUT_BASE_DIR not defined")
    endif()
    set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/${CONFIG_DIR}/lib)
    if (WIN32 OR APPLE)
        set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/${CONFIG_DIR}/bin)
        if (BUILD_SHARED_LIBS)
            target_compile_definitions(${target} PRIVATE _USRDLL)
        endif()
    elseif (UNIX)
        set_target_properties(${target} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/${CONFIG_DIR}/lib)
    endif()
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/${CONFIG_DIR}/bin)
endfunction()

function(setup_target_properties_executable target)
    if ("${OUTPUT_BASE_DIR}" STREQUAL "")
        message(ERROR "Invalid: OUTPUT_BASE_DIR not defined")
    endif()
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR}/${CONFIG_DIR}/bin)
    if (UNIX)
        set_target_properties(${target} PROPERTIES LINK_FLAGS -Wl,-rpath,'\$ORIGIN/../lib/')
    endif()
endfunction()

macro(display_package_variables)
    display_list("Package description               : " ${PACKAGE_DESCRIPTION} )
    display_list("Package version major             : " ${PACKAGE_VERSION_MAJOR} )
    display_list("Package version minor             : " ${PACKAGE_VERSION_MINOR} )
    display_list("Package version micro             : " ${PACKAGE_VERSION_MICRO} )
    display_list("Package version                   : " ${PACKAGE_VERSION} )
    display_list("Build reference                   : " ${PACKAGE_BUILD_REFERENCE} )
    display_list("Defines                           : " ${PACKAGE_COMPILER_DEFINITIONS} )
    display_list("Defines - private                 : " ${PACKAGE_COMPILER_DEFINITIONS_PRIVATE} )
    display_list("Compiler options                  : " ${PACKAGE_COMPILER_OPTIONS} )
    display_list("Source files                      : " ${PACKAGE_SOURCES} )
    display_list("Include files (public)            : " ${PACKAGE_INCLUDES_PUBLIC} )
    display_list("Include files (private)           : " ${PACKAGE_INCLUDES_PRIVATE} )
    display_list("Include dirs                      : " ${PACKAGE_INCLUDE_DIRS} )
    display_list("Link libs                         : " ${PACKAGE_LIBS} )
    display_list("Linker options                    : " ${PACKAGE_LINK_OPTIONS} )
    display_list("Dependencies                      : " ${PACKAGE_DEPENDENCIES} )
endmacro()

macro(define_target_library_shared)
    display_package_variables()
        
    # add the library
    add_library(${PACKAGE_NAME} SHARED ${PACKAGE_SOURCES} ${PACKAGE_INCLUDES_PUBLIC} ${PACKAGE_INCLUDES_PRIVATE})
    target_link_libraries(${PACKAGE_NAME} ${PACKAGE_LIBS})
    target_include_directories(${PACKAGE_NAME} PUBLIC ${PACKAGE_INCLUDE_DIRS})
    target_compile_definitions(${PACKAGE_NAME} PRIVATE ${PACKAGE_COMPILER_DEFINITIONS_PRIVATE})
    target_compile_definitions(${PACKAGE_NAME} PUBLIC ${PACKAGE_COMPILER_DEFINITIONS})
    if (PACKAGE_BUILD_REFERENCE)
        target_compile_definitions(${PACKAGE_NAME} PUBLIC BUILD_REFERENCE=${PACKAGE_BUILD_REFERENCE})
    endif()
    target_compile_options(${PACKAGE_NAME} PRIVATE ${PACKAGE_COMPILER_OPTIONS})

    list_to_string(PACKAGE_LINK_OPTIONS PACKAGE_LINK_OPTIONS_STRING)
    if (NOT "${PACKAGE_LINK_OPTIONS_STRING}" STREQUAL "")
        set_target_properties(${MODULE_NAME} PROPERTIES LINK_FLAGS "${PACKAGE_LINK_OPTIONS}")
    endif()
    set_target_properties(${PACKAGE_NAME} PROPERTIES LINK_FLAGS "${PACKAGE_LINK_OPTIONS_STRING}")
    set_target_properties(${PACKAGE_NAME} PROPERTIES VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})
    set_target_properties(${PACKAGE_NAME} PROPERTIES SOVERSION ${PACKAGE_VERSION_MAJOR})
    set_target_properties(${PACKAGE_NAME} PROPERTIES OUTPUT_NAME ${TARGET_NAME})
    setup_target_properties_library(${PACKAGE_NAME})
endmacro()

macro(define_target_executable)
    display_package_variables()

    # add the library
    add_executable(${PACKAGE_NAME} ${PACKAGE_SOURCES} ${PACKAGE_INCLUDES_PUBLIC} ${PACKAGE_INCLUDES_PRIVATE})
    target_link_libraries(${PACKAGE_NAME} ${PACKAGE_LIBS})
    target_include_directories(${PACKAGE_NAME} PUBLIC ${PACKAGE_INCLUDE_DIRS})
    target_compile_definitions(${PACKAGE_NAME} PRIVATE ${PACKAGE_COMPILER_DEFINITIONS_PRIVATE})
    target_compile_definitions(${PACKAGE_NAME} PUBLIC ${PACKAGE_COMPILER_DEFINITIONS})
    if (PACKAGE_BUILD_REFERENCE)
        target_compile_definitions(${PACKAGE_NAME} PUBLIC BUILD_REFERENCE=${PACKAGE_BUILD_REFERENCE})
    endif()
    target_compile_options(${PACKAGE_NAME} PRIVATE ${PACKAGE_COMPILER_OPTIONS})

    list_to_string(PACKAGE_LINK_OPTIONS PACKAGE_LINK_OPTIONS_STRING)
    if (NOT "${PACKAGE_LINK_OPTIONS_STRING}" STREQUAL "")
        set_target_properties(${MODULE_NAME} PROPERTIES LINK_FLAGS "${PACKAGE_LINK_OPTIONS}")
    endif()
    set_target_properties(${PACKAGE_NAME} PROPERTIES LINK_FLAGS "${PACKAGE_LINK_OPTIONS_STRING}")
    set_target_properties(${PACKAGE_NAME} PROPERTIES VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})
    set_target_properties(${PACKAGE_NAME} PROPERTIES SOVERSION ${PACKAGE_VERSION_MAJOR})
    set_target_properties(${PACKAGE_NAME} PROPERTIES OUTPUT_NAME ${TARGET_NAME})
    setup_target_properties_library(${PACKAGE_NAME})
endmacro()

function(add_uninstall_target TARGET MAIN_TARGET)
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/uninstall.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
        IMMEDIATE @ONLY)

    add_custom_target(uninstall_${TARGET}
        COMMAND ${CMAKE_COMMAND} -DCOMPONENT=${TARGET} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake
        DEPENDS ${TARGET}
        COMMENT "Uninstalling ${TARGET}")
    add_dependencies(${MAIN_TARGET} uninstall_${TARGET})
endfunction()

function(add_install_target TARGET MAIN_TARGET)
    add_custom_target(install_${TARGET}
        COMMAND ${CMAKE_COMMAND} -DCOMPONENT=${TARGET} -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
        DEPENDS ${TARGET}
        COMMENT "Installing ${TARGET}")
    add_dependencies(${MAIN_TARGET} install_${TARGET})
endfunction()

function(install_pkgconfig_file PC_FILE)
    configure_file(
        ${PC_FILE}.in
        ${PC_FILE}
        @ONLY)
    install(FILES ${PC_FILE}
        DESTINATION ${PKGCONFIG_FILE_PKG_CONFIG_DIR}
        COMPONENT ${PACKAGE_NAME})
endfunction()
