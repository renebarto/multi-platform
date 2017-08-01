#pragma once

#include <osal/PlatformDefines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    #if defined(CRYPTO_DYNAMIC)
        #if defined(CRYPTO_BUILD)
            #pragma message("crypto Windows DLL export")
            #define CRYPTO_EXPORT __declspec(dllexport)
        #else // !defined(CRYPTO_BUILD)
            #pragma message("crypto Windows DLL import")
            #define CRYPTO_EXPORT __declspec(dllimport)
        #endif // defined(CRYPTO_BUILD)
    #else // !defined(CRYPTO_DYNAMIC)
        #pragma message("crypto Windows Lib")
        #define CRYPTO_EXPORT
    #endif // defined(CRYPTO_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
#define CRYPTO_EXPORT
#else
#error "Unknown platform"
#endif
