#pragma once

#include <osal/PlatformDefines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
#if defined(DATABASE_DYNAMIC)
#if defined(DATABASE_BUILD)
//#pragma message("core Windows DLL export")
#define DATABASE_EXPORT __declspec(dllexport)
#else // !defined(DATABASE_BUILD)
//#pragma message("core Windows DLL import")
#define DATABASE_EXPORT __declspec(dllimport)
#endif // defined(DATABASE_BUILD)
#else // !defined(DATABASE_DYNAMIC)
//#pragma message("core Windows Lib")
#define DATABASE_EXPORT
#endif // defined(DATABASE_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
#define DATABASE_EXPORT
#else
#error "Unknown platform"
#endif

