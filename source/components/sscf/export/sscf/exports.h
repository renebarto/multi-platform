#pragma once

#include <osal/PlatformDefines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
#if defined(SSCF_DYNAMIC)
#if defined(SSCF_BUILD)
//#pragma message("sscf Windows DLL export")
#define SSCF_EXPORT __declspec(dllexport)
#else // !defined(SSCF_BUILD)
//#pragma message("sscf Windows DLL import")
#define SSCF_EXPORT __declspec(dllimport)
#endif // defined(SSCF_BUILD)
#else // !defined(SSCF_DYNAMIC)
//#pragma message("sscf Windows Lib")
#define SSCF_EXPORT
#endif // defined(SSCF_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
#define SSCF_EXPORT
#else
#error "Unknown platform"
#endif

