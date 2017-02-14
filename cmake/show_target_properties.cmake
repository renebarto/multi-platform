function(show_target_properties)
    message(STATUS "Build type:                         " ${CMAKE_BUILD_TYPE})

    get_target_property(DEFINES ${PROJECT_NAME} COMPILE_DEFINITIONS)
    list_to_string(DEFINES STR)
    message(STATUS "Target defines:                     " ${STR})

    get_target_property(OPTIONS ${PROJECT_NAME} COMPILE_OPTIONS)
    list_to_string(OPTIONS STR)
    message(STATUS "Target options:                     " ${STR})

    get_target_property(INCLUDES ${PROJECT_NAME} INCLUDE_DIRECTORIES)
    list_to_string(INCLUDES STR)
    message(STATUS "Target includes:                    " ${STR})

    get_target_property(LIBRARIES ${PROJECT_NAME} LINK_LIBRARIES)
    list_to_string(LIBRARIES STR)
    message(STATUS "Target link libraries:              " ${STR})

    get_target_property(LIBRARIES ${PROJECT_NAME} LINK_FLAGS)
    list_to_string(LIBRARIES STR)
    message(STATUS "Target link options:                " ${STR})

    get_target_property(DEFINES_EXPORTS ${PROJECT_NAME} INTERFACE_COMPILE_DEFINITIONS)
    list_to_string(DEFINES_EXPORTS STR)
    message(STATUS "Target exported defines:            " ${STR})

    get_target_property(OPTIONS_EXPORTS ${PROJECT_NAME} INTERFACE_COMPILE_OPTIONS)
    list_to_string(OPTIONS_EXPORTS STR)
    message(STATUS "Target exported options:            " ${STR})

    get_target_property(INCLUDES_EXPORTS ${PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES)
    list_to_string(INCLUDES_EXPORTS STR)
    message(STATUS "Target exported includes:           " ${STR})

    get_target_property(LIBRARIES_EXPORTS ${PROJECT_NAME} INTERFACE_LINK_LIBRARIES)
    list_to_string(LIBRARIES_EXPORTS STR)
    message(STATUS "Target exported link libraries:     " ${STR})

    get_test_property(IMPORT_DEPENDENCIES ${PROJECT_NAME} IMPORTED_LINK_DEPENDENT_LIBRARIES)
    list_to_string(LIBRARIES_EXPORTS STR)
    message(STATUS "Target imported dependencies:       " ${STR})

    get_test_property(IMPORT_LIBRARIES ${PROJECT_NAME} IMPORTED_LINK_INTERFACE_LIBRARIES)
    list_to_string(LIBRARIES_EXPORTS STR)
    message(STATUS "Target imported link libraries:     " ${STR})

    get_target_property(ARCHIVE_LOCATION ${PROJECT_NAME} ARCHIVE_OUTPUT_DIRECTORY)
    message(STATUS "Target static library location:     " ${ARCHIVE_LOCATION})

    get_target_property(LIBRARY_LOCATION ${PROJECT_NAME} LIBRARY_OUTPUT_DIRECTORY)
    message(STATUS "Target dynamic library location:    " ${LIBRARY_LOCATION})

    get_target_property(RUNTIME_LOCATION ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
    message(STATUS "Target binary location:             " ${RUNTIME_LOCATION})


endfunction()
