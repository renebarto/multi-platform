#pragma once

#include "osal/platform-defines.h"

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    #if defined(OSAL_DYNAMIC)
        #if defined(OSAL_BUILD)
            #pragma message("osal Windows DLL export")
            #define OSAL_EXPORT __declspec(dllexport)
        #else // !defined(OSAL_BUILD)
            #pragma message("osal Windows DLL import")
            #define OSAL_EXPORT __declspec(dllimport)
        #endif // defined(OSAL_BUILD)
    #else // !defined(OSAL_DYNAMIC)
        #pragma message("osal Windows Lib")
        #define OSAL_EXPORT
    #endif // defined(OSAL_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
#define OSAL_EXPORT
#else
#error "Unknown platform"
#endif
