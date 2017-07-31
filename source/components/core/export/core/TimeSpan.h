#pragma once

#include <cstdint>
#include <string>
#include "core/Core.h"

namespace Core
{

class TimeSpan
{
public:
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

    virtual OSAL::String ToString() const;

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
