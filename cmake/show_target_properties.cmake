include (${CMAKE_SOURCE_DIR}/cmake/display_list.cmake)

function(show_target_properties target)
    message(STATUS "Properties for ${target}")

    get_target_property(DEFINES ${target} COMPILE_DEFINITIONS)
    display_list("Target defines                    : " ${DEFINES})

    get_target_property(OPTIONS ${target} COMPILE_OPTIONS)
    display_list("Target options                    : " ${OPTIONS})

    get_target_property(INCLUDES ${target} INCLUDE_DIRECTORIES)
    display_list("Target includes                   : " ${INCLUDES})

    get_target_property(LIBRARIES ${target} LINK_LIBRARIES)
    display_list("Target link libraries             : " ${LIBRARIES})

    get_target_property(LINK_OPTIONS ${target} LINK_FLAGS)
    display_list("Target link options               : " ${LINK_OPTIONS})

    get_target_property(DEFINES_EXPORTS ${target} INTERFACE_COMPILE_DEFINITIONS)
    display_list("Target exported defines           : " ${DEFINES_EXPORTS})

    get_target_property(OPTIONS_EXPORTS ${target} INTERFACE_COMPILE_OPTIONS)
    display_list("Target exported options           : " ${OPTIONS_EXPORTS})

    get_target_property(INCLUDE_DIRS_EXPORTS ${target} INTERFACE_INCLUDE_DIRECTORIES)
    display_list("Target exported includes          : " ${INCLUDE_DIRS_EXPORTS})

    get_target_property(LIBRARIES_EXPORTS ${target} INTERFACE_LINK_LIBRARIES)
    display_list("Target exported link libraries    : " ${LIBRARIES_EXPORTS})

    get_test_property(IMPORT_DEPENDENCIES ${target} IMPORTED_LINK_DEPENDENT_LIBRARIES)
    display_list("Target imported dependencies      : " ${IMPORT_DEPENDENCIES})

    get_test_property(IMPORT_LIBRARIES ${target} IMPORTED_LINK_INTERFACE_LIBRARIES)
    display_list("Target imported link libraries    : " ${IMPORT_LIBRARIES})

    get_target_property(LINK_DEPENDENCIES ${target} LINK_DEPENDS)
    display_list("Target link dependencies          : " ${LINK_DEPENDENCIES})

    get_target_property(EXPLICIT_DEPENDENCIES ${target} MANUALLY_ADDED_DEPENDENCIES)
    display_list("Target manual dependencies        : " ${EXPLICIT_DEPENDENCIES})

    get_target_property(ARCHIVE_LOCATION ${target} ARCHIVE_OUTPUT_DIRECTORY)
    display_list("Target static library location    : " ${ARCHIVE_LOCATION})

    get_target_property(LIBRARY_LOCATION ${target} LIBRARY_OUTPUT_DIRECTORY)
    display_list("Target dynamic library location   : " ${LIBRARY_LOCATION})

    get_target_property(RUNTIME_LOCATION ${target} RUNTIME_OUTPUT_DIRECTORY)
    display_list("Target binary location            : " ${RUNTIME_LOCATION})

    get_target_property(TARGET_LINK_FLAGS ${target} LINK_FLAGS)
    display_list("Target link flags                 : " ${TARGET_LINK_FLAGS})

    get_target_property(TARGET_VERSION ${target} VERSION)
    display_list("Target version                    : " ${TARGET_VERSION})

    get_target_property(TARGET_SOVERSION ${target} SOVERSION)
    display_list("Target so-version                 : " ${TARGET_SOVERSION})

    get_target_property(TARGET_OUTPUT_NAME ${target} OUTPUT_NAME)
    display_list("Target output name                : " ${TARGET_OUTPUT_NAME})

endfunction()
