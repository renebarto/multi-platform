#include "osal/Time.h"

#include <climits>
#include "osal/Strings.h"

namespace OSAL
{
namespace Time
{

const tm & BaseTime()
{
    static tm baseTime;
    return baseTime;
}

const long tm::tm_tzOffset = 0;
const int tm::tm_dstOffset = 0;
const OSAL::Char tm::tm_tzName[MAX_TIME_ZONE_NAME + 1] = _("");
static constexpr int SecondsPerMinute = 60;

tm::tm(bool initialize)
: _tm()
{
    if (initialize)
        Update();
}

tm::tm(const tm & other)
: _tm(other._tm)
{
}

tm::tm(int second, int minute, int hour, int day, int month, int year, bool initialize)
: _tm()
{
    if (initialize)
        Update();
    _tm.tm_sec = second;
    _tm.tm_min = minute;
    _tm.tm_hour = hour;
    _tm.tm_mday = day;
    _tm.tm_mon = month;
    _tm.tm_year = year;
}

void tm::Update()
{
    tzset();
    *(const_cast<long *>(&tm_tzOffset)) = ::timezone;
    *(const_cast<int *>(&tm_dstOffset)) = ::daylight;
    OSAL::Strings::strncpy(const_cast<OSAL::Char *>(tm_tzName), ::tzname[0],
                           MAX_TIME_ZONE_NAME + 1);
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
