#pragma once

#include <osal/PlatformDefines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    #if defined(XML_DYNAMIC)
        #if defined(XML_BUILD)
            #pragma message("xml Windows DLL export")
            #define XML_EXPORT __declspec(dllexport)
        #else // !defined(XML_BUILD)
            #pragma message("xml Windows DLL import")
            #define XML_EXPORT __declspec(dllimport)
        #endif // defined(XML_BUILD)
    #else // !defined(XML_DYNAMIC)
        #pragma message("xml Windows Lib")
        #define XML_EXPORT
    #endif // defined(XML_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
#define XML_EXPORT
#else
#error "Unknown platform"
#endif
