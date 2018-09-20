#pragma once

#include <cstdint>
#include <time.h>
#include "osal/Unused.h"
#include "osal/windows/OSAL.h"
#include "osal/Strings.h"

namespace OSAL {
namespace Time {

/* FILETIME of Jan 1 1970 00:00:00. */
constexpr unsigned __int64 epoch = ((unsigned __int64)116444736000000000ULL);
constexpr size_t MAX_TIME_ZONE_NAME = 4;

class timespec : public ::timespec
{
public:
    timespec()
        : ::timespec()
    {}
    timespec(long seconds, long nanoSeconds)
        : ::timespec{ seconds, nanoSeconds }
    {
    }
    timespec(const ::timespec & time)
        : ::timespec(time)
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

inline int GetTimeOfDay(struct timeval *time, struct timezone *UNUSED(timeZone))
{
    FILETIME file_time;
    SYSTEMTIME system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.u.LowPart = file_time.dwLowDateTime;
    ularge.u.HighPart = file_time.dwHighDateTime;

    time->tv_sec = (long) ((ularge.QuadPart - epoch) / 10000000L);
    time->tv_usec = (long) (system_time.wMilliseconds * 1000);

    return 0;
}

inline bool Sleep(const LARGE_INTEGER & timerValue)
{
    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (!timer)
        return false;
    if (!SetWaitableTimer(timer, &timerValue, 0, NULL, NULL, 0))
        return false;
    DWORD result = WaitForSingleObject(timer, INFINITE);
    if ((result == WAIT_FAILED) || (result == WAIT_TIMEOUT))
        return false;
    CloseHandle(timer);
    return true;
}

inline int MicroSleep(int64_t microSeconds)
{
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * microSeconds); // Convert to 100 nanosecond interval, negative value indicates relative time
    return Sleep(ft) ? 0 : -1;
}

inline int NanoSleep(const timespec * req, timespec * UNUSED(rem))
{
    LARGE_INTEGER ft;
    constexpr int64_t NanoSecondsPerSecond = 1000000000;
    ft.QuadPart = -(((req->tv_sec * NanoSecondsPerSecond + req->tv_nsec) + 99) /
                    100); // Convert to 100 nanosecond interval, negative value indicates relative time

    return Sleep(ft) ? 0 : -1;
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
    long ActiveDaylightSavingsTimeOffsetSeconds() const { return _tm.tm_isdst ? tm_dstOffset : 0; }
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

inline std::wstring strftime(const wchar_t * format, const tm * time)
{
    constexpr size_t BufferSize = 4096;
    wchar_t buffer[BufferSize];
    size_t charsWritten = ::wcsftime(buffer, BufferSize, format, &(time->GetTime()));
    return std::wstring(buffer, charsWritten);
}

// Time / date
enum clockid_t
{
    // Identifier for system-wide realtime clock.
    CLOCK_REALTIME = 0,
    // Monotonic system-wide clock.
    CLOCK_MONOTONIC = 1,
    // High-resolution timer from the CPU.
    CLOCK_PROCESS_CPUTIME_ID = 2,
    // Thread-specific CPU-time clock.
    CLOCK_THREAD_CPUTIME_ID = 3,
    // Monotonic system-wide clock, not adjusted for frequency scaling.
    CLOCK_MONOTONIC_RAW = 4,
    // Identifier for system-wide realtime clock, updated only on ticks.
    CLOCK_REALTIME_COARSE = 5,
    // Monotonic system-wide clock, updated only on ticks.
    CLOCK_MONOTONIC_COARSE = 6,
};

inline int ClockGetResolution(clockid_t clockID, OSAL::Time::timespec * res)
{
    if (!res)
        return EFAULT;
    if (clockID != CLOCK_REALTIME)
        return EINVAL;
    LARGE_INTEGER performanceFrequency;
    BOOL result = QueryPerformanceFrequency(&performanceFrequency);
    res->tv_sec = long(performanceFrequency.QuadPart / 1000000000);
    res->tv_nsec = long(performanceFrequency.QuadPart % 1000000000);
    return result ? 0 : EINVAL;
}

OSAL_EXPORT int ClockGetTime(clockid_t clockID, OSAL::Time::timespec * tp);

} // namespace Time
} // namespace OSAL