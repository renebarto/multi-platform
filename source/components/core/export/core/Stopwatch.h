#pragma once

#include <time.h>
#include <string>
#include <iostream>
#include <core/Core.h>

namespace Core
{

class CORE_EXPORT Stopwatch
{
public:
    Stopwatch();
    virtual ~Stopwatch();

    void Start();
    void Lap();
    OSAL::Time::timespec GetStartTime();
    OSAL::Time::timespec GetLapTime();
    OSAL::Time::timespec GetResolution();
    double GetElapsedTime();

    std::ostream & PrintTo(std::ostream & stream) const;

protected:
    OSAL::Time::timespec startTime;
    OSAL::Time::timespec lapTime;
    OSAL::Time::timespec freq;
};

inline void PrintTo(const Stopwatch & value, std::ostream & stream)
{
    value.PrintTo(stream);
}

} // namespace Tools
