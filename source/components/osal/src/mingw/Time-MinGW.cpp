#include "osal/Time.h"

#include <climits>
#include <direct.h>

namespace OSAL {
namespace Time {

long tm::tm_tzOffset = 0;
int tm::tm_dstOffset = 0;
char tm::tm_tzName[MAX_TIME_ZONE_NAME + 1] = _("");
char tm::tm_tzNameDst[MAX_TIME_ZONE_NAME + 1] = _("");
bool tm::initialized = false;
static constexpr int SecondsPerMinute = 60;

tm::tm()
    : _tm()
{
    if (!initialized)
        Initialize();
}

tm::tm(const tm &other)
    : _tm(other._tm)
{
}

tm::tm(int second, int minute, int hour, int day, int month, int year)
    : _tm()
{
    if (!initialized)
        Initialize();
    _tm.tm_sec = second;
    _tm.tm_min = minute;
    _tm.tm_hour = hour;
    _tm.tm_mday = day;
    _tm.tm_mon = month;
    _tm.tm_year = year;
}

void tm::Initialize()
{
    TIME_ZONE_INFORMATION timeZoneInformation;
    /* DWORD result = */ GetTimeZoneInformation(&timeZoneInformation);
    tm_tzOffset= SecondsPerMinute * (timeZoneInformation.Bias + timeZoneInformation.StandardBias);
    tm_dstOffset= SecondsPerMinute * timeZoneInformation.DaylightBias;
    OSAL::Strings::strncpy(tm_tzName, ToString(timeZoneInformation.StandardName).c_str(), MAX_TIME_ZONE_NAME + 1);
    OSAL::Strings::strncpy(tm_tzNameDst, ToString(timeZoneInformation.DaylightName).c_str(), MAX_TIME_ZONE_NAME + 1);
    initialized = true;
}

tm & tm::operator=(const tm & other)
{
    if (&other != this)
    {
        _tm = other._tm;
    }
    return *this;
}

tm & tm::operator=(const ::tm & other)
{
    _tm = other;
    return *this;
}

} // namespace Time
} // namespace OSAL
