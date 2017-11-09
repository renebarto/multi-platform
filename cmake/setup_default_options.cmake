include(${CMAKE_SOURCE_DIR}/cmake/platform.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/list_to_string.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/convert_default_options.cmake)

# Defines the default compiler options for each platform:
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
# DEFINES                   Compiler definitions for all platforms, for langurage C and C++
# DEFINES_DEBUG             Compiler definitions for debug build, for langurage C and C++
# DEFINES_RELEASE           Compiler definitions for release build, for langurage C and C++
# DEFINES_MINSIZEREL        Compiler definitions for release min size build, for langurage C and C++
# DEFINES_RELWITHDEBINFO    Compiler definitions for release with debug info build, for langurage C and C++
# LINK_FLAGS                Linker specific options
# LINK_LIBRARIES            Generic libraries to be linked

macro(setup_default_options)
    if(WIN_MSVC)
        # Important! do not keep warning 4251 4275 ignored!
        # Possibly switch warning 4505 back on
        set(FLAGS_CXX
            /Wall /wd4251 /wd4275 /wd4505 /wd4514 /wd4548 /wd4571 /wd4592 /wd4625 /wd4626 /wd4710
            /wd4820 /wd5026 /wd5027 /WX- /EHsc /Gd /GR /sdl- /Zc:wchar_t /Zc:inline /fp:precise)
        set(FLAGS_CXX_DEBUG /Od /Gm- /ZI /RTC1 /MDd)
        set(FLAGS_CXX_RELEASE /Ox /GL /GS /Gy /Oi /MD)
        set(FLAGS_CXX_MINSIZEREL /O1 /GL /GS /Gy /Oi /MD)
        set(FLAGS_CXX_RELWITHDEBINFO /O2 /GL /GS /Gy /Oi /Zi /MD)

        set(FLAGS_C
            /Wall /wd4251 /wd4275 /wd4505 /wd4514 /wd4548 /wd4571 /wd4625 /wd4626 /wd4710
            /wd4820 /wd5026 /wd5027 /WX- /EHsc /Gd /GR /sdl- /Zc:wchar_t /Zc:inline /fp:precise)
        set(FLAGS_C_DEBUG /Od /Gm- /ZI /RTC1 /MDd)
        set(FLAGS_C_RELEASE /Ox /GL /GS /Gy /Oi /MD)
        set(FLAGS_C_MINSIZEREL /O1 /GL /GS /Gy /Oi /MD)
        set(FLAGS_C_RELWITHDEBINFO /O2 /GL /GS /Gy /Oi /Zi /MD)

        set(DEFINES _UNICODE UNICODE _CRT_SECURE_NO_WARNINGS)
        set(DEFINES_DEBUG _DEBUG)
        set(DEFINES_RELEASE NDEBUG)
        set(DEFINES_MINSIZEREL NDEBUG)
        set(DEFINES_RELWITHDEBINFO NDEBUG)

        set(LINK_FLAGS )
        set(LINK_LIBRARIES )

        if (MEASURE_COVERAGE)
            message(STATUS "Adding options for coverage")
        endif()

    elseif(MINGW)
        set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC -fexceptions -fmessage-length=0)
        set(FLAGS_CXX_DEBUG -O0 -g)
        set(FLAGS_CXX_RELEASE -O3)
        set(FLAGS_CXX_MINSIZEREL -O3)
        set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

        set(FLAGS_C -Wall -Wextra -fPIC -fmessage-length=0)
        set(FLAGS_C_DEBUG -O0 -g)
        set(FLAGS_C_RELEASE -O3)
        set(FLAGS_C_MINSIZEREL -O3)
        set(FLAGS_C_RELWITHDEBINFO -O2 -g)

        set(DEFINES )
        set(DEFINES_DEBUG _DEBUG)
        set(DEFINES_RELEASE NDEBUG)
        set(DEFINES_MINSIZEREL NDEBUG)
        set(DEFINES_RELWITHDEBINFO NDEBUG)

        set(LINK_FLAGS )
        set(LINK_LIBRARIES )

        if (MEASURE_COVERAGE)
            message(STATUS "Adding options for coverage")
        endif()

    elseif(LINUX)
        set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC -fexceptions -fmessage-length=0)
        set(FLAGS_CXX_DEBUG -O0 -g)
        set(FLAGS_CXX_RELEASE -O3)
        set(FLAGS_CXX_MINSIZEREL -O3)
        set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

        set(FLAGS_C -Wall -Wextra -fPIC -fmessage-length=0)
        set(FLAGS_C_DEBUG -O0 -g)
        set(FLAGS_C_RELEASE -O3)
        set(FLAGS_C_MINSIZEREL -O3)
        set(FLAGS_C_RELWITHDEBINFO -O2 -g)

        set(DEFINES )
        set(DEFINES_DEBUG _DEBUG)
        set(DEFINES_RELEASE NDEBUG)
        set(DEFINES_MINSIZEREL NDEBUG)
        set(DEFINES_RELWITHDEBINFO NDEBUG)

        set(LINK_FLAGS )
        set(LINK_LIBRARIES )

        if (MEASURE_COVERAGE)
            message(STATUS "Adding options for coverage")
            list(APPEND FLAGS_C -fprofile-arcs -ftest-coverage)
            list(APPEND FLAGS_CXX -fprofile-arcs -ftest-coverage)
            list(APPEND LINK_FLAGS --coverage)
            list(APPEND LINK_LIBRARIES gcov)
        endif()

    elseif(APPLE)
        set(FLAGS_CXX -std=c++11 -Wall -Wextra -fPIC -fexceptions -fmessage-length=0)
        set(FLAGS_CXX_DEBUG -O0 -g)
        set(FLAGS_CXX_RELEASE -O3)
        set(FLAGS_CXX_MINSIZEREL -O3)
        set(FLAGS_CXX_RELWITHDEBINFO -O2 -g)

        set(FLAGS_C -Wall -Wextra -fPIC -fmessage-length=0)
        set(FLAGS_C_DEBUG -O0 -g)
        set(FLAGS_C_RELEASE -O3)
        set(FLAGS_C_MINSIZEREL -O3)
        set(FLAGS_C_RELWITHDEBINFO -O2 -g)

        set(DEFINES )
        set(DEFINES_DEBUG _DEBUG)
        set(DEFINES_RELEASE NDEBUG)
        set(DEFINES_MINSIZEREL NDEBUG)
        set(DEFINES_RELWITHDEBINFO NDEBUG)

        set(LINK_FLAGS )
        set(LINK_LIBRARIES )

        if (MEASURE_COVERAGE)
            message(STATUS "Adding options for coverage")
            list(APPEND FLAGS_C -fprofile-arcs -ftest-coverage)
            list(APPEND FLAGS_CXX -fprofile-arcs -ftest-coverage)
            list(APPEND LINK_FLAGS --coverage)
            list(APPEND LINK_LIBRARIES gcov)
        endif()

    else()
        display_list("Unsupported platform " ${CMAKE_HOST_SYSTEM})
        return()
    endif()

    set(CMAKE_COMPILER_IS_CLANG OFF)
    if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        display_list("Using Clang")
        set(CMAKE_COMPILER_IS_CLANG ON)
    elseif(${CMAKE_COMPILER_IS_GNUCXX})
        display_list("Using Gcc")
    else()
        display_list("Compiler " ${CMAKE_CXX_COMPILER_ID})
    endif()

    if(CMAKE_USE_PTHREADS_INIT)
        display_list("Adding pthread flags")
        list(APPEND FLAGS_CXX -pthread)
        list(APPEND FLAGS_C -pthread)
    endif()

    if(CMAKE_VERBOSE_MAKEFILE)
        display_list("Defines:                            " ${DEFINES})
        display_list("Defines - debug:                    " ${DEFINES_DEBUG})
        display_list("Defines - release:                  " ${DEFINES_RELEASE})
        display_list("Defines - release min size:         " ${DEFINES_MINSIZEREL})
        display_list("Defines - release debug info:       " ${DEFINES_RELWITHDEBINFO})

        display_list("C++ Flags:                          " ${FLAGS_CXX})
        display_list("C++ Flags - debug:                  " ${FLAGS_CXX_DEBUG})
        display_list("C++ Flags - release:                " ${FLAGS_CXX_RELEASE})
        display_list("C++ Flags - release min size:       " ${FLAGS_CXX_MINSIZEREL})
        display_list("C++ Flags - release debug info:     " ${FLAGS_CXX_RELWITHDEBINFO})

        display_list("C Flags:                            " ${FLAGS_C})
        display_list("C Flags - debug:                    " ${FLAGS_C_DEBUG})
        display_list("C Flags - release:                  " ${FLAGS_C_RELEASE})
        display_list("C Flags - release min size:         " ${FLAGS_C_MINSIZEREL})
        display_list("C Flags - release debug info:       " ${FLAGS_C_RELWITHDEBINFO})

        display_list("Link Flags:                         " ${LINK_FLAGS})
        display_list("Link Libraries:                     " ${LINK_LIBRARIES})
    endif()
    convert_default_options()
endmacro()
