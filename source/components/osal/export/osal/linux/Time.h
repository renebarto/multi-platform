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
    timespec()
        : ::timespec()
    {}
    timespec(long seconds, long nanoSeconds)
            : ::timespec { seconds, nanoSeconds }
    {
    }

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
    void PrintTo(std::ostream & stream) const
    {
        stream << "tv_sec=" << tv_sec << " tv_nsec=" << tv_nsec;
    }
};

inline std::ostream & operator << (std::ostream & stream, const timespec & value)
{
    value.PrintTo(stream);
    return stream;
}

inline int GetTimeOfDay(struct timeval * time, struct timezone * timeZone)
{
    return ::gettimeofday(time, timeZone);
}

inline int MicroSleep(int64_t microSeconds)
{
    return ::usleep(static_cast<__useconds_t>(microSeconds));
}

inline int NanoSleep(const timespec * req, timespec * rem)
{
    return ::nanosleep(req, rem);
}

using TimeValSeconds = long;
using TimeValMicroSeconds = long;

struct OSAL_EXPORT tm
{
    tm();
    tm(const ::tm & other, bool localTime);
    tm(const tm & other);
    tm(int second, int minute, int hour, int day, int month, int year, bool localTime);

    void Initialize();

    tm & operator=(const tm & other);
    tm & operator=(const ::tm & other);

    long ActiveLocalTimeOffsetSeconds() const { return tm_tzOffset + (_tm.tm_isdst ? tm_dstOffset : 0); }
    static long LocalTimeOffsetSecondsNonDaylightSavings() { return tm_tzOffset; }
    static long LocalTimeOffsetSecondsDaylightSavings() { return tm_tzOffset + tm_dstOffset; }
    std::string ActiveTimeZoneName() const { return _tm.tm_isdst ? tm_tzNameDst : tm_tzName; };
    static std::string NonDaylightSavingsTimeZoneName() { return tm_tzName; }
    static std::string DaylightSavingsActiveTimeZoneName() { return tm_tzNameDst; }

    ::tm & GetTime() { return _tm; }
    const ::tm & GetTime() const { return _tm; }

    bool IsEmpty() const;

private:
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
    return ::mktime(&(tim->GetTime()));
}

inline std::string strftime(const char * format, const tm * time)
{
    constexpr size_t BufferSize = 4096;
    char buffer[BufferSize];
    size_t charsWritten = ::strftime(buffer, BufferSize, format, &(time->GetTime()));
    return std::string(buffer, charsWritten);
}

inline int ClockGetResolution(clockid_t clockID, OSAL::Time::timespec * res)
{
    return ::clock_getres(clockID, res);
}

inline int ClockGetTime(clockid_t clockID, OSAL::Time::timespec * tp)
{
    return ::clock_gettime(clockID, tp);
}

} // namespace Time
} // namespace OSAL
