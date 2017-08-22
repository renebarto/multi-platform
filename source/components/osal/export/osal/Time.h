#pragma once

#include <osal/exports.h>

#if defined(WIN_MSVC)
#include "osal/windows/Time.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Time.h"
#elif defined(DARWIN)
#include "osal/darwin/Time.h"
#elif defined(LINUX)
#include "osal/linux/Time.h"
#endif

namespace OSAL {
namespace Time {

inline timespec ConvertTimeOffsetToTimeSpec(int offsetMS)
{
    timespec time;
    OSAL::Time::clock_gettime(CLOCK_REALTIME, &time);
    time.tv_nsec += (offsetMS % 1000) * 1000000;
    time.tv_sec += offsetMS / 1000;
    time.tv_sec += time.tv_nsec / 1000000000;
    time.tv_nsec %= 1000000000;
    return time;
}

} // namespace Time
} // namespace OSAL
