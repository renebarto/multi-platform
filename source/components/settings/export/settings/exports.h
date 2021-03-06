#pragma once

#include <osal/PlatformDefines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    #if defined(CRYPTO_DYNAMIC)
        #if defined(CRYPTO_BUILD)
            #pragma message("json Windows DLL export")
            #define SETTINGS_EXPORT __declspec(dllexport)
        #else // !defined(CRYPTO_BUILD)
            #pragma message("json Windows DLL import")
            #define SETTINGS_EXPORT __declspec(dllimport)
        #endif // defined(CRYPTO_BUILD)
    #else // !defined(CRYPTO_DYNAMIC)
//        #pragma message("json Windows Lib")
        #define SETTINGS_EXPORT
    #endif // defined(CRYPTO_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
#define SETTINGS_EXPORT
#else
#error "Unknown platform"
#endif
