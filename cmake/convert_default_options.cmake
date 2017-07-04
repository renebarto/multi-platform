include(${CMAKE_SOURCE_DIR}/cmake/display_list.cmake)

# Sets up the default compiler options, based on a number of defined variables:
# FLAGS_CXX                 Compiler flags for all platforms, for langurage C++
# FLAGS_CXX_DEBUG           Compiler flags for debug build, for langurage C++
# FLAGS_CXX_RELEASE         Compiler flags for release build, for langurage C++
# FLAGS_CXX_MINSIZEREL      Compiler flags for release min size build, for langurage C++
# FLAGS_CXX_RELWITHDEBINFO  Compiler flags for release with debug info build, for langurage C++
# FLAGS_C                   Compiler flags for all platforms, for langurage C
# FLAGS_C_DEBUG             Compiler flags for debug build, for langurage C
# FLAGS_C_RELEASE           Compiler flags for release build, for langurage C
# FLAGS_C_MINSIZEREL        Compiler flags for release min size build, for langurage C
# FLAGS_C_RELWITHDEBINFO    Compiler flags for release with debug info build, for langurage C
# COMPILER_DEFINITIONS                   Compiler definitions for all platforms, for langurage C and C++
# COMPILER_DEFINITIONS_DEBUG             Compiler definitions for debug build, for langurage C and C++
# COMPILER_DEFINITIONS_RELEASE           Compiler definitions for release build, for langurage C and C++
# COMPILER_DEFINITIONS_MINSIZEREL        Compiler definitions for release min size build, for langurage C and C++
# COMPILER_DEFINITIONS_RELWITHDEBINFO    Compiler definitions for release with debug info build, for langurage C and C++
#
# The following standard variables are set and placed in the cache (by appending the contents of the mentioned
# lists, and subsequently converting the lists to space separated string):
#
# COMPILER_DEFINITIONS                           COMPILER_DEFINITIONS
# COMPILER_DEFINITIONS_DEBUG                     COMPILER_DEFINITIONS COMPILER_DEFINITIONS_DEBUG
# COMPILER_DEFINITIONS_RELEASE                   COMPILER_DEFINITIONS COMPILER_DEFINITIONS_RELEASE
# COMPILER_DEFINITIONS_MINSIZEREL                COMPILER_DEFINITIONS COMPILER_DEFINITIONS_MINSIZEREL
# COMPILER_DEFINITIONS_RELWITHDEBINFO            COMPILER_DEFINITIONS COMPILER_DEFINITIONS_RELWITHDEBINFO
# COMPILER_OPTIONS_CXX_FLAGS                 FLAGS_CXX
# COMPILER_OPTIONS_CXX_FLAGS_DEBUG           FLAGS_CXX FLAGS_CXX_DEBUG
# COMPILER_OPTIONS_CXX_FLAGS_RELEASE         FLAGS_CXX FLAGS_CXX_RELEASE
# COMPILER_OPTIONS_CXX_FLAGS_MINSIZEREL      FLAGS_CXX FLAGS_CXX_MINSIZEREL
# COMPILER_OPTIONS_CXX_FLAGS_RELWITHDEBINFO  FLAGS_CXX FLAGS_CXX_RELWITHDEBINFO
# COMPILER_OPTIONS_C_FLAGS                   FLAGS_C COMPILER_DEFINITIONS
# COMPILER_OPTIONS_C_FLAGS_DEBUG             FLAGS_C FLAGS_C_DEBUG
# COMPILER_OPTIONS_C_FLAGS_RELEASE           FLAGS_C FLAGS_C_RELEASE
# COMPILER_OPTIONS_C_FLAGS_MINSIZEREL        FLAGS_C FLAGS_C_MINSIZEREL
# COMPILER_OPTIONS_C_FLAGS_RELWITHDEBINFO    FLAGS_C FLAGS_C_RELWITHDEBINFO

macro(convert_default_options)
    set(COMPILER_DEFINITIONS_DEBUG ${DEFINES} ${DEFINES_DEBUG})
    set(COMPILER_DEFINITIONS_RELEASE ${DEFINES} ${DEFINES_RELEASE})
    set(COMPILER_DEFINITIONS_MINSIZEREL ${DEFINES} ${DEFINES_MINSIZEREL})
    set(COMPILER_DEFINITIONS_RELWITHDEBINFO ${DEFINES} ${DEFINES_RELWITHDEBINFO})

    set(COMPILER_OPTIONS_CXX_DEBUG ${FLAGS_CXX} ${FLAGS_CXX_DEBUG})
    set(COMPILER_OPTIONS_CXX_RELEASE ${FLAGS_CXX} ${FLAGS_CXX_RELEASE})
    set(COMPILER_OPTIONS_CXX_MINSIZEREL ${FLAGS_CXX} ${FLAGS_CXX_MINSIZEREL})
    set(COMPILER_OPTIONS_CXX_RELWITHDEBINFO ${FLAGS_CXX} ${FLAGS_CXX_RELWITHDEBINFO})

    set(COMPILER_OPTIONS_C_DEBUG ${FLAGS_C} ${FLAGS_C_DEBUG})
    set(COMPILER_OPTIONS_C_RELEASE ${FLAGS_C} ${FLAGS_C_RELEASE})
    set(COMPILER_OPTIONS_C_MINSIZEREL ${FLAGS_C} ${FLAGS_C_MINSIZEREL})
    set(COMPILER_OPTIONS_C_RELWITHDEBINFO ${FLAGS_C} ${FLAGS_C_RELWITHDEBINFO})

    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(COMPILER_DEFINITIONS ${COMPILER_DEFINITIONS_DEBUG})
        set(COMPILER_OPTIONS_CXX ${COMPILER_OPTIONS_CXX_DEBUG})
        set(COMPILER_OPTIONS_C ${COMPILER_OPTIONS_C_DEBUG})
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set(COMPILER_DEFINITIONS ${COMPILER_DEFINITIONS_RELEASE})
        set(COMPILER_OPTIONS_CXX ${COMPILER_OPTIONS_CXX_RELEASE})
        set(COMPILER_OPTIONS_C ${COMPILER_OPTIONS_CXX_RELEASE})
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
        set(COMPILER_DEFINITIONS ${COMPILER_DEFINITIONS_MINSIZEREL})
        set(COMPILER_OPTIONS_CXX ${COMPILER_OPTIONS_CXX_MINSIZEREL})
        set(COMPILER_OPTIONS_C ${COMPILER_OPTIONS_C_MINSIZEREL})
    elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
        set(COMPILER_DEFINITIONS ${COMPILER_DEFINITIONS_RELWITHDEBINFO})
        set(COMPILER_OPTIONS_CXX ${COMPILER_OPTIONS_CXX_RELWITHDEBINFO})
        set(COMPILER_OPTIONS_C ${COMPILER_OPTIONS_C_RELWITHDEBINFO})
    else()
        message(FATAL_ERROR "Invalid build type: " ${CMAKE_BUILD_TYPE})
    endif()

    if(CMAKE_VERBOSE_MAKEFILE)
        display_list("COMPILER_DEFINITIONS_DEBUG            : " ${COMPILER_DEFINITIONS_DEBUG})
        display_list("COMPILER_DEFINITIONS_RELEASE          : " ${COMPILER_DEFINITIONS_RELEASE})
        display_list("COMPILER_DEFINITIONS_MINSIZEREL       : " ${COMPILER_DEFINITIONS_MINSIZEREL})
        display_list("COMPILER_DEFINITIONS_RELWITHDEBINFO   : " ${COMPILER_DEFINITIONS_RELWITHDEBINFO})

        display_list("COMPILER_OPTIONS_CXX_DEBUG            : " ${COMPILER_OPTIONS_CXX_DEBUG})
        display_list("COMPILER_OPTIONS_CXX_RELEASE          : " ${COMPILER_OPTIONS_CXX_RELEASE})
        display_list("COMPILER_OPTIONS_CXX_MINSIZEREL       : " ${COMPILER_OPTIONS_CXX_MINSIZEREL})
        display_list("COMPILER_OPTIONS_CXX_RELWITHDEBINFO   : " ${COMPILER_OPTIONS_CXX_RELWITHDEBINFO})

        display_list("COMPILER_OPTIONS_C_DEBUG              : " ${COMPILER_OPTIONS_C_DEBUG})
        display_list("COMPILER_OPTIONS_C_RELEASE            : " ${COMPILER_OPTIONS_C_RELEASE})
        display_list("COMPILER_OPTIONS_C_MINSIZEREL         : " ${COMPILER_OPTIONS_C_MINSIZEREL})
        display_list("COMPILER_OPTIONS_C_RELWITHDEBINFO     : " ${COMPILER_OPTIONS_C_RELWITHDEBINFO})

        display_list("Active COMPILER_DEFINITIONS           : " ${COMPILER_DEFINITIONS})
        display_list("Active COMPILER_OPTIONS_CXX           : " ${COMPILER_OPTIONS_CXX})
        display_list("Active COMPILER_OPTIONS_C             : " ${COMPILER_OPTIONS_C})
    endif()
endmacro()
