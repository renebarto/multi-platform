#pragma once

#include <cstdint>

#pragma warning(disable : 4668)
#include <windows.h>
#pragma warning(default : 4668)

#include <time.h>
#include "osal/Unused.h"

namespace OSAL {
namespace Time {

/* FILETIME of Jan 1 1970 00:00:00. */
constexpr unsigned __int64 epoch = ((unsigned __int64)116444736000000000ULL);
constexpr int MinutesPerHour = 60;
constexpr int SecondsPerMinute = 60;
constexpr size_t MAX_TIME_ZONE_NAME = 4;

inline int GetTimeOfDay(struct timeval * time, struct timezone * UNUSED(timeZone))
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
	tm(bool Initialize = false);
	tm(const tm & other);
	tm(int second, int minute, int hour, int day, int month, int year, bool initialize = false);
	void Update();
	tm & operator = (const tm & other);
	tm & operator = (const ::tm & other);
	
	struct ::tm _tm;
    static const long tm_tzOffset;
	static const int tm_dstOffset;
	static const char tm_tzName[MAX_TIME_ZONE_NAME + 1];
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