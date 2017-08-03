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

inline int gettimeofday(struct timeval * time, struct timezone * UNUSED(timeZone))
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

inline void usleep(int64_t microSeconds)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * microSeconds); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

inline int nanosleep(const timespec * req, timespec * rem)
{
    HANDLE timer;
    LARGE_INTEGER ft;
    constexpr int64_t NanoSecondsPerSeconds = 1000000000;
    ft.QuadPart = -(((req->tv_sec * NanoSecondsPerSeconds + req->tv_nsec) + 99) /
                    100); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    if (rem)
    {
        rem->tv_sec = 0;
        rem->tv_nsec = 0;
    }
    CloseHandle(timer);
    return 0;
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

    struct ::tm _tm;
    // tm_tzOffset is defined as the number of seconds to add to result in UTC. For Western European time, this is -3600
    static long tm_tzOffset;
    // tm_dstOffset is defined as the number of seconds to add to result in standard time. For Western European time, this is -3600
    static int tm_dstOffset;
    static OSAL::Char tm_tzName[MAX_TIME_ZONE_NAME + 1];
    static OSAL::Char tm_tzNameDst[MAX_TIME_ZONE_NAME + 1];
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

// Time / date
// Identifier for system-wide realtime clock.
#define CLOCK_REALTIME       0
// Monotonic system-wide clock.
#define CLOCK_MONOTONIC      1
// High-resolution timer from the CPU.
#define CLOCK_PROCESS_CPUTIME_ID 2
// Thread-specific CPU-time clock.
#define CLOCK_THREAD_CPUTIME_ID  3
// Monotonic system-wide clock, not adjusted for frequency scaling.
#define CLOCK_MONOTONIC_RAW      4
// Identifier for system-wide realtime clock, updated only on ticks.
#define CLOCK_REALTIME_COARSE    5
// Monotonic system-wide clock, updated only on ticks.
#define CLOCK_MONOTONIC_COARSE   6

inline int clock_getres(int X, struct timespec * tv)
{
    if (!tv)
        return EFAULT;
    if (X != CLOCK_REALTIME)
        return EINVAL;
    LARGE_INTEGER performanceFrequency;
    BOOL result = QueryPerformanceFrequency(&performanceFrequency);
    tv->tv_sec = long(performanceFrequency.QuadPart / 1000000000);
    tv->tv_nsec = long(performanceFrequency.QuadPart % 1000000000);
    return result ? 0 : EINVAL;
}

OSAL_EXPORT int clock_gettime(int X, struct timespec * tv);

} // namespace Time
} // namespace OSAL