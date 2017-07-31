#pragma once

#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <osal/OSAL.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
#if defined(CORE_DYNAMIC)
#if defined(CORE_BUILD)
#pragma message("core Windows DLL export")
#define __declspec(dllexport)
#else // !defined(CORE_BUILD)
#pragma message("core Windows DLL import")
#define __declspec(dllimport)
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

namespace Core
{

extern const OSAL::String ComponentName;

constexpr int64_t NanoSecondsPerMicroSecond = 1000;
constexpr int64_t MicroSecondsPerMilliSecond = 1000;
constexpr int64_t MilliSecondsPerSecond = 1000;
constexpr int64_t NanoSecondsPerMilliSecond = MicroSecondsPerMilliSecond * NanoSecondsPerMicroSecond;
constexpr int64_t NanoSecondsPerSecond = MilliSecondsPerSecond * NanoSecondsPerMilliSecond;
constexpr int64_t MicroSecondsPerSecond = MicroSecondsPerMilliSecond * MilliSecondsPerSecond;
constexpr int HoursPerDay = 24;
constexpr int MinutesPerHour = 60;
constexpr int SecondsPerMinute = 60;
constexpr int SecondsPerHour = MinutesPerHour * SecondsPerMinute;
constexpr int SecondsPerDay = HoursPerDay * SecondsPerHour;
constexpr int64_t NanoSecondsPerMinute = SecondsPerMinute * NanoSecondsPerSecond;
constexpr int64_t NanoSecondsPerHour = MinutesPerHour * NanoSecondsPerMinute;
constexpr int64_t NanoSecondsPerDay = HoursPerDay * NanoSecondsPerHour;

} // namespace Core
