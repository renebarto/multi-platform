#pragma once

#include <osal/platform-defines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    #if defined(UNIT_TEST_CPP_DYNAMIC)
        #if defined(UNIT_TEST_CPP_BUILD)
            #if defined(WIN_MSVC)
                #pragma message("unit-test-c++ Windows DLL export")
            #endif
            #define UNIT_TEST_CPP_EXPORT __declspec(dllexport)
        #else // !defined(UNIT_TEST_CPP_BUILD)
            #if defined(WIN_MSVC)
                #pragma message("unit-test-c++ Windows DLL import")
            #endif
            #define UNIT_TEST_CPP_EXPORT __declspec(dllimport)
        #endif // defined(UNIT_TEST_CPP_BUILD)
    #else // !defined(UNIT_TEST_CPP_DYNAMIC)
        #if defined(WIN_MSVC)
            #pragma message("unit-test-c++ Windows Lib")
        #endif
        #define UNIT_TEST_CPP_EXPORT
    #endif // defined(UNIT_TEST_CPP_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
    #define UNIT_TEST_CPP_EXPORT
#else
    #error "Unknown platform"
#endif
