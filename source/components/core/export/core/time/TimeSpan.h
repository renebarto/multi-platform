#pragma once

#include <iostream>
#include "core/exports.h"

namespace Core {
namespace Time {

class CORE_EXPORT TimeSpan
{
public:
    static constexpr int64_t NanoSecondsPerMicroSecond = 1000;
    static constexpr int64_t NanoSecondsPerMilliSecond = 1000 * TimeSpan::NanoSecondsPerMicroSecond;
    static constexpr int64_t NanoSecondsPerSecond = 1000 * TimeSpan::NanoSecondsPerMilliSecond;
    static constexpr int64_t MicroSecondsPerSecond = 1000000;
    static constexpr int64_t NanoSecondsPerMinute = 60 * TimeSpan::NanoSecondsPerSecond;
    static constexpr int64_t NanoSecondsPerHour = 60 * TimeSpan::NanoSecondsPerMinute;
    static constexpr int64_t NanoSecondsPerDay = 24 * TimeSpan::NanoSecondsPerHour;;

    TimeSpan();
    explicit TimeSpan(int64_t intervalNanoSeconds);
    TimeSpan(const TimeSpan & other);
    virtual ~TimeSpan();

    TimeSpan & operator = (int64_t intervalNanoSeconds);
    TimeSpan & operator = (const TimeSpan & other);

    friend bool operator == (const TimeSpan & lhs, const TimeSpan & rhs);
    friend bool operator == (int64_t intervalNanoSeconds, const TimeSpan & rhs);
    friend bool operator == (const TimeSpan & lhs, int64_t intervalNanoSeconds);
    friend bool operator != (const TimeSpan & lhs, const TimeSpan & rhs);
    friend bool operator != (int64_t intervalNanoSeconds, const TimeSpan & rhs);
    friend bool operator != (const TimeSpan & lhs, int64_t intervalNanoSeconds);

    operator int64_t () const;
    int64_t NanoSeconds() const;
    int64_t MicroSeconds() const;
    int64_t MilliSeconds() const;
    double Seconds() const;
    int Minutes() const;
    int Hours() const;
    int Days() const;

    void PrintTo(std::ostream & stream) const;

private:
    int64_t _intervalNanoSeconds;
};

inline void PrintTo(TimeSpan const & timeSpan, std::ostream & stream)
{
    timeSpan.PrintTo(stream);
}

} // namespace Time
} // namespace Core
