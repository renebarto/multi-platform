#pragma once

#include <cstdint>
#include <string>

namespace Core
{

class TimeSpan
{
public:
    static const int64_t NanoSecondsPerMicroSecond;
    static const int64_t NanoSecondsPerMilliSecond;
    static const int64_t MicroSecondsPerSecond;
    static const int64_t NanoSecondsPerSecond;
    static const int64_t NanoSecondsPerMinute;
    static const int64_t NanoSecondsPerHour;
    static const int64_t NanoSecondsPerDay;

    TimeSpan();
    TimeSpan(int64_t interval);
    TimeSpan(const TimeSpan & other);
    virtual ~TimeSpan();

    TimeSpan & operator = (int64_t interval);
    TimeSpan & operator = (const TimeSpan & other);

    friend bool operator == (const TimeSpan & lhs, const TimeSpan & rhs);
    friend bool operator == (int64_t lhs, const TimeSpan & rhs);
    friend bool operator == (const TimeSpan & lhs, int64_t rhs);
    friend bool operator != (const TimeSpan & lhs, const TimeSpan & rhs);
    friend bool operator != (int64_t lhs, const TimeSpan & rhs);
    friend bool operator != (const TimeSpan & lhs, int64_t rhs);

    operator int64_t () const;
    int64_t NanoSeconds() const;
    int64_t MicroSeconds() const;
    int64_t MilliSeconds() const;
    double Seconds() const;
    int Minutes() const;
    int Hours() const;
    int Days() const;

    virtual std::string ToString() const;

private:
    int64_t _interval;
};

bool operator == (const TimeSpan & lhs, const TimeSpan & rhs);
bool operator == (int64_t lhs, const TimeSpan & rhs);
bool operator == (const TimeSpan & lhs, int64_t rhs);
bool operator != (const TimeSpan & lhs, const TimeSpan & rhs);
bool operator != (int64_t lhs, const TimeSpan & rhs);
bool operator != (const TimeSpan & lhs, int64_t rhs);

} // namespace Core
