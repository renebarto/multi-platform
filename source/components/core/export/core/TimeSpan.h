#pragma once

#include <cstdint>
#include <string>
#include "core/Core.h"

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

    virtual OSAL::String ToString() const;

private:
    int64_t _interval;
};

CORE_EXPORT bool operator == (const TimeSpan & lhs, const TimeSpan & rhs);
CORE_EXPORT bool operator == (int64_t lhs, const TimeSpan & rhs);
CORE_EXPORT bool operator == (const TimeSpan & lhs, int64_t rhs);
CORE_EXPORT bool operator != (const TimeSpan & lhs, const TimeSpan & rhs);
CORE_EXPORT bool operator != (int64_t lhs, const TimeSpan & rhs);
CORE_EXPORT bool operator != (const TimeSpan & lhs, int64_t rhs);

} // namespace Core
