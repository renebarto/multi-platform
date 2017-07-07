#pragma once

#undef WIN_MSVC
#undef WIN_MINGW
#undef LINUX
#undef DARWIN

#if defined(_MSC_VER)
    #define WIN_MSVC 1
#elif defined(__GNUC__)
    #if defined(__MINGW32__)
        #define WIN_MINGW 1
    #elif defined(__APPLE__) && defined(__MACH__)
        #define DARWIN 1
    #else
        #define LINUX 1
    #endif
#endif