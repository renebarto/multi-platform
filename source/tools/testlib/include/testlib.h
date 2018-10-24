#pragma once

#include <osal/PlatformDefines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
#if defined(TESTLIB_DYNAMIC)
        #if defined(TESTLIB_BUILD)
//            #pragma message("osal Windows DLL export")
            #define TESTLIB_EXPORT __declspec(dllexport)
        #else // !defined(TESTLIB_BUILD)
//            #pragma message("osal Windows DLL import")
            #define TESTLIB_EXPORT __declspec(dllimport)
        #endif // defined(TESTLIB_BUILD)
    #else // !defined(TESTLIB_DYNAMIC)
//        #pragma message("osal Windows Lib")
        #define TESTLIB_EXPORT
    #endif // defined(TESTLIB_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
#define TESTLIB_EXPORT
#else
#error "Unknown platform"
#endif

#ifdef __cplusplus
extern "C" {
#endif

TESTLIB_EXPORT int TheAnswer();

#ifdef __cplusplus
}
#endif