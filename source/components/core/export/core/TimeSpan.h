#pragma once

#include <cstdint>
#include <iomanip>
#include <string>
#include "core/core.h"

namespace Core
{

class CORE_EXPORT TimeSpan
{
public:
    TimeSpan();
    TimeSpan(int64_t interval);
    TimeSpan(const TimeSpan & other);
    virtual ~TimeSpan();

    TimeSpan & operator = (int64_t interval);
    TimeSpan & operator = (const TimeSpan & other);

    friend CORE_EXPORT bool operator == (const TimeSpan & lhs, const TimeSpan & rhs);
    friend CORE_EXPORT bool operator == (int64_t lhs, const TimeSpan & rhs);
    friend CORE_EXPORT bool operator == (const TimeSpan & lhs, int64_t rhs);
    friend CORE_EXPORT bool operator != (const TimeSpan & lhs, const TimeSpan & rhs);
    friend CORE_EXPORT bool operator != (int64_t lhs, const TimeSpan & rhs);
    friend CORE_EXPORT bool operator != (const TimeSpan & lhs, int64_t rhs);

    operator int64_t () const;
    int64_t NanoSeconds() const;
    int64_t MicroSeconds() const;
    int64_t MilliSeconds() const;
    double Seconds() const;
    int Minutes() const;
    int Hours() const;
    int Days() const;

    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const
    {
        int days = static_cast<int>(_interval / NanoSecondsPerDay);
        int hours = static_cast<int>((_interval / NanoSecondsPerHour) % HoursPerDay);
        int minutes = static_cast<int>((_interval / NanoSecondsPerMinute) % MinutesPerHour);
        int seconds = static_cast<int>((_interval / NanoSecondsPerSecond) % SecondsPerMinute);
        int microSeconds = static_cast<int>((_interval / NanoSecondsPerMicroSecond) % MicroSecondsPerSecond);
        s << days << " days, " << hours << " hours, "
          << minutes << " minutes, "
          << seconds << "." << std::setfill('0') << std::setw(6) << microSeconds << " seconds";
        return s;
    }

private:
    int64_t _interval;
};

CORE_EXPORT bool operator == (const TimeSpan & lhs, const TimeSpan & rhs);
CORE_EXPORT bool operator == (int64_t lhs, const TimeSpan & rhs);
CORE_EXPORT bool operator == (const TimeSpan & lhs, int64_t rhs);
CORE_EXPORT bool operator != (const TimeSpan & lhs, const TimeSpan & rhs);
CORE_EXPORT bool operator != (int64_t lhs, const TimeSpan & rhs);
CORE_EXPORT bool operator != (const TimeSpan & lhs, int64_t rhs);

template <class Elem, class Traits>
inline void PrintTo(std::basic_ostream<Elem, Traits> & stream, const TimeSpan & value)
{
    value.PrintTo(stream);
}

template <class Elem, class Traits>
inline std::basic_ostream<Elem, Traits> & operator << (std::basic_ostream<Elem, Traits> & stream, const TimeSpan & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace Core
