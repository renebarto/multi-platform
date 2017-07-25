#pragma once

#include <cstdint>
#include <windows.h>
#include <time.h>

namespace OSAL {
namespace Time {

/* FILETIME of Jan 1 1970 00:00:00. */
constexpr unsigned __int64 epoch = ((unsigned __int64)116444736000000000ULL);

inline int GetTimeOfDay(struct timeval * time, struct timezone * timeZone)
{
    FILETIME    file_time;
    SYSTEMTIME  system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.u.LowPart = file_time.dwLowDateTime;
    ularge.u.HighPart = file_time.dwHighDateTime;

    time->tv_sec = (long)((ularge.QuadPart - epoch) / 10000000L);
    time->tv_usec = (long)(system_time.wMilliseconds * 1000);

    return 0;
}
inline void USleep(int64_t microSeconds)
{
    HANDLE timer;
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * microSeconds); // Convert to 100 nanosecond interval, negative value indicates relative time

    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}

struct tm
{
    tm()
        : _tm()
    {
        Update();
    }
    tm(int second, int minute, int hour, int day, int month, int year)
        : _tm()
    {
        _tm.tm_sec = second;
        _tm.tm_min = minute;
        _tm.tm_hour = hour;
        _tm.tm_mday = day;
        _tm.tm_mon = month;
        _tm.tm_year = year;
    }
    void Update()
    {
        tm_tzOffset = *::__timezone();
        tm_dstOffset = ::daylight;
        tm_tzName = *::__tzname();;
    }
    tm & operator = (const ::tm & other)
    {
        _tm = other;
        Update();
        return *this;
    }
    struct ::tm _tm;
    long tm_tzOffset;
    int tm_dstOffset;
    const char * tm_tzName;
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

} // namespace Time
} // namespace OSAL