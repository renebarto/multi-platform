#pragma once

#include <unistd.h>
#include <sys/time.h>
#include <cstdint>
#include "osal/Strings.h"

namespace OSAL {
namespace Time {

constexpr size_t MAX_TIME_ZONE_NAME = 4;

inline int gettimeofday(struct timeval * time, struct timezone * timeZone)
{
    return ::gettimeofday(time, timeZone);
}

inline int usleep(int64_t microSeconds)
{
    return ::usleep(static_cast<useconds_t>(microSeconds));
}

inline int nanosleep(const timespec * req, timespec * rem)
{
    return ::nanosleep(req, rem);
}

using TimeValSeconds = __darwin_time_t;
using TimeValMicroSeconds = __darwin_suseconds_t;

struct OSAL_EXPORT timeval
{
    TimeValSeconds tv_sec;
    TimeValMicroSeconds tv_usec;
};

struct OSAL_EXPORT tm
{
    tm(bool initialize = false);

    tm(const tm & other);

    tm(int second, int minute, int hour, int day, int month, int year, bool initialize = false);

    void Update();

    tm & operator=(const tm & other);

    tm & operator=(const ::tm & other);

    struct ::tm _tm;
    // tm_tzOffset is defined as the number of seconds to add to result in UTC. For Western European time, this is -3600
    static const long tm_tzOffset;
    // tm_dstOffset is defined as the number of seconds to add to result in standard time. For Western European time, this is -3600
    static const int tm_dstOffset;
    static const OSAL::Char tm_tzName[MAX_TIME_ZONE_NAME + 1];
};

inline tm * localtime(const time_t * timep)
{
    static tm _time;
    _time = *::localtime(timep);
    return &_time;
}

inline tm * gmtime(const time_t * timep)
{
    static tm _time;
    _time = *::gmtime(timep);
    return &_time;
}

inline time_t mktime(tm * tim)
{
    return ::mktime(&(tim->_tm));
}

inline size_t strftime(char * strDest, size_t maxSize, const char * format, const ::tm * time)
{
    return ::strftime(strDest, maxSize, format, time);
}

inline size_t strftime(wchar_t * strDest, size_t maxSize, const wchar_t * format, const ::tm * time)
{
    return ::wcsftime(strDest, maxSize, format, time);
}

inline int clock_getres(clockid_t clockID, struct timespec * res)
{
    return ::clock_getres(clockID, res);
}

inline int clock_gettime(clockid_t clockID, struct timespec * tp)
{
    return ::clock_gettime(clockID, tp);
}

} // namespace Time
} // namespace OSAL