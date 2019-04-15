#pragma once

#include <osal/PlatformDefines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
#if defined(REMOTING_DYNAMIC)
#if defined(REMOTING_BUILD)
//#pragma message("remoting Windows DLL export")
#define REMOTING_EXPORT __declspec(dllexport)
#else // !defined(REMOTING_BUILD)
//#pragma message("remoting Windows DLL import")
#define REMOTING_EXPORT __declspec(dllimport)
#endif // defined(REMOTING_BUILD)
#else // !defined(REMOTING_DYNAMIC)
//#pragma message("remoting Windows Lib")
#define REMOTING_EXPORT
#endif // defined(REMOTING_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
#define REMOTING_EXPORT
#else
#error "Unknown platform"
#endif

