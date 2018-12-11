#pragma once

#include <time.h>
#include <string>
#include <iomanip>
#include <iostream>
#include "core/core.h"
#include <osal/Time.h>

namespace Core
{

class CORE_EXPORT Stopwatch final
{
public:
    Stopwatch();
    ~Stopwatch();

    void Start();
    void Lap();
    OSAL::Time::timespec GetStartTime();
    OSAL::Time::timespec GetLapTime();
    OSAL::Time::timespec GetResolution();
    double GetElapsedTime();

    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const
    {
        s << "start = " << _startTime.tv_sec << "." << std::setfill('0') << std::setw(9) << _startTime.tv_nsec
          << " lap = " << _lapTime.tv_sec << "." << std::setfill('0') << std::setw(9) << _lapTime.tv_nsec;
        return s;
    }

protected:
    OSAL::Time::timespec _startTime;
    OSAL::Time::timespec _lapTime;
    OSAL::Time::timespec _freq;
};

template <class Elem, class Traits>
inline void PrintTo(std::basic_ostream<Elem, Traits> & stream, const Stopwatch & value)
{
    value.PrintTo(stream);
}

template <class Elem, class Traits>
inline std::basic_ostream<Elem, Traits> & operator << (std::basic_ostream<Elem, Traits> & stream, const Stopwatch & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace Tools
