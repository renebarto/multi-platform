#pragma once

#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <cstdint>
#include <iostream>
#include "osal/Strings.h"

namespace OSAL {
namespace Time {

constexpr size_t MAX_TIME_ZONE_NAME = 4;

class timespec : public ::timespec
{
public:
    timespec() = default;

    inline bool operator == (const timespec & other) const
    {
        return (tv_sec == other.tv_sec) && (tv_nsec == other.tv_nsec);
    }

    inline bool operator != (const timespec & other) const
    {
        return !operator== (other);
    }

    inline bool operator > (const timespec & other) const
    {
        return (tv_sec > other.tv_sec) || ((tv_sec == other.tv_sec) && (tv_nsec > other.tv_nsec));
    }

    inline bool operator >= (const timespec & other) const
    {
        return !operator < (other);
    }

    inline bool operator < (const timespec & other) const
    {
        return (tv_sec < other.tv_sec) || ((tv_sec == other.tv_sec) && (tv_nsec < other.tv_nsec));
    }

    inline bool operator <= (const timespec & other) const
    {
        return !operator > (other);
    }
    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const
    {
        s << "tv_sec=" << tv_sec << " tv_nsec=" << tv_nsec;
        return s;
    }
};

template <class Elem, class Traits>
inline void PrintTo(std::basic_ostream<Elem, Traits> & stream, const timespec & value)
{
    value.PrintTo(stream);
}

template <class Elem, class Traits>
inline std::basic_ostream<Elem, Traits> & operator << (std::basic_ostream<Elem, Traits> & stream, const timespec & value)
{
    value.PrintTo(stream);
    return stream;
}

inline int gettimeofday(struct timeval * time, struct timezone * timeZone)
{
    return ::gettimeofday(time, timeZone);
}

inline int usleep(int64_t microSeconds)
{
    return ::usleep(static_cast<__useconds_t>(microSeconds));
}

inline int nanosleep(const timespec * req, timespec * rem)
{
    return ::nanosleep(req, rem);
}

// Implementation for dependable sleep. Due to timers firing, the normal sleep is interrupted,
// and it is advised to use clock_nanosleep with an absolute time to correctly wait without stalling.
template<class Rep, class Period>
void Sleep(std::chrono::duration<Rep, Period> sleep)
{
    uint64_t NanoSecondsPerSecond = 1000000000;
    timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    timespec requested;
    uint64_t nanoSeconds = std::chrono::nanoseconds(sleep);
    requested.tv_sec = now.tv_sec + (nanoSeconds / NanoSecondsPerSecond);
    requested.tv_nsec = now.tv_nsec + (nanoSeconds % NanoSecondsPerSecond);
    requested.tv_sec += requested.tv_nsec / NanoSecondsPerSecond;
    requested.tv_nsec = requested.tv_nsec % NanoSecondsPerSecond;

    bool done = false;
    int result {};
    do
    {
        result = ::clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &requested, nullptr);
        if (result == 0)
        {
            done = true;
            continue;
        }
        if (result == EINTR)
            continue;
        return;
    }
    while (!done);

}

using TimeValSeconds = long;
using TimeValMicroSeconds = long;

struct OSAL_EXPORT tm
{
    tm();

    tm(const tm & other);

    tm(int second, int minute, int hour, int day, int month, int year);

    void Initialize();

    tm & operator=(const tm & other);

    tm & operator=(const ::tm & other);

    static long LocalTimeOffsetSeconds() { return tm_tzOffset; }
    struct ::tm _tm;
    // tm_tzOffset is defined as the number of seconds to add to result in UTC. For Western European time, this is -3600
    static long tm_tzOffset;
    // tm_dstOffset is defined as the number of seconds to add to result in standard time. For Western European time, this is -3600
    static int tm_dstOffset;
    static char tm_tzName[MAX_TIME_ZONE_NAME + 1];
    static char tm_tzNameDst[MAX_TIME_ZONE_NAME + 1];
    static bool initialized;
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

inline int clock_getres(clockid_t clockID, OSAL::Time::timespec * res)
{
    return ::clock_getres(clockID, res);
}

inline int clock_gettime(clockid_t clockID, OSAL::Time::timespec * tp)
{
    return ::clock_gettime(clockID, tp);
}

} // namespace Time
} // namespace OSAL
