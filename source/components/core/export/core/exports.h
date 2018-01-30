#pragma once

#include <osal/PlatformDefines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
#if defined(CORE_DYNAMIC)
#if defined(CORE_BUILD)
#pragma message("core Windows DLL export")
#define CORE_EXPORT __declspec(dllexport)
#else // !defined(CORE_BUILD)
#pragma message("core Windows DLL import")
#define CORE_EXPORT __declspec(dllimport)
#endif // defined(CORE_BUILD)
#else // !defined(CORE_DYNAMIC)
#pragma message("core Windows Lib")
#define CORE_EXPORT
#endif // defined(CORE_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
#define CORE_EXPORT
#else
#error "Unknown platform"
#endif

