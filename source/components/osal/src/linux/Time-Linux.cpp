#include "osal/Time.h"

#include "osal/Strings.h"
#include <climits>

namespace OSAL {
namespace Time {

long tm::tm_tzOffset                          = 0;
int tm::tm_dstOffset                          = 0;
char tm::tm_tzName[MAX_TIME_ZONE_NAME + 1]    = "";
char tm::tm_tzNameDst[MAX_TIME_ZONE_NAME + 1] = "";
bool tm::initialized                          = false;

static constexpr int SecondsPerMinute = 60;

tm::tm()
    : _tm()
{
    if (!initialized)
        Initialize();
}

tm::tm(const ::tm & other, bool localTime)
    : _tm(other)
{
    if (!initialized)
        Initialize();
    if (!IsEmpty())
    {
        time_t epochTime = mktime(this);
        _tm = localTime ? *::localtime(&epochTime) : *::gmtime(&epochTime);
    }
}

tm::tm(const tm & other)
    : _tm(other._tm)
{
}

tm::tm(int second, int minute, int hour, int day, int month, int year, bool localTime)
    : _tm()
{
    if (!initialized)
        Initialize();
    _tm.tm_sec  = second;
    _tm.tm_min  = minute;
    _tm.tm_hour = hour;
    _tm.tm_mday = day;
    _tm.tm_mon  = month - 1;
    _tm.tm_year = year - 1900;
    time_t epochTime = mktime(this);
    if (!localTime)
        epochTime -= ActiveLocalTimeOffsetSeconds();
    else
        epochTime += ActiveDaylightSavingsTimeOffsetSeconds();
    _tm = localTime ? *::localtime(&epochTime) : *::gmtime(&epochTime);
}

void tm::Initialize()
{
    tzset();
    tm_tzOffset  = ::timezone;
    tm_dstOffset = -3600;
    OSAL::Strings::strncpy(tm_tzName, ::tzname[0], MAX_TIME_ZONE_NAME + 1);
    OSAL::Strings::strncpy(tm_tzNameDst, ::tzname[1], MAX_TIME_ZONE_NAME + 1);
    initialized = true;
}
tm & tm::operator = (const tm & other)
{
    if (&other != this)
    {
        _tm = other._tm;
    }
    return *this;
}

tm & tm::operator = (const ::tm & other)
{
    _tm = other;
    return *this;
}

bool tm::IsEmpty() const
{
    return (_tm.tm_year == 0) &&
           (_tm.tm_mon == 0) &&
           (_tm.tm_mday == 0) &&
           (_tm.tm_hour == 0) &&
           (_tm.tm_min == 0) &&
           (_tm.tm_sec == 0) &&
           (_tm.tm_yday == 0) &&
           (_tm.tm_wday == 0) &&
           (_tm.tm_isdst == 0);
}

} // namespace Time
} // namespace OSAL
