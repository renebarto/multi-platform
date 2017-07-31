#pragma once

#include <cstring>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <osal/OSAL.h>
#include "core/Exports.h"

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
