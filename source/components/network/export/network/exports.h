#pragma once

#include <osal/PlatformDefines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    #if defined(NETWORK_DYNAMIC)
        #if defined(NETWORK_BUILD)
//            #pragma message("network Windows DLL export")
            #define NETWORK_EXPORT __declspec(dllexport)
        #else // !defined(NETWORK_BUILD)
//            #pragma message("network Windows DLL import")
            #define NETWORK_EXPORT __declspec(dllimport)
        #endif // defined(NETWORK_BUILD)
    #else // !defined(NETWORK_DYNAMIC)
        #pragma message("network Windows Lib")
        #define NETWORK_EXPORT
    #endif // defined(NETWORK_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
#define NETWORK_EXPORT
#else
#error "Unknown platform"
#endif
