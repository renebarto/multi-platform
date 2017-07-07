#pragma once

#include <unistd.h>
#include <sys/time.h>

namespace OSAL {
namespace Time {

inline int GetTimeOfDay(struct timeval * time, struct timezone * timeZone)
{
    return ::gettimeofday(time, timeZone);
}
inline void USleep(int64_t microSeconds)
{
    ::usleep(microSeconds);
}

} // namespace Time
} // namespace OSAL