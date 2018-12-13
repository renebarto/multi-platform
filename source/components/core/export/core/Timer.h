#pragma once

#include <chrono>
#include <functional>
#include <string>
#include <iomanip>
#include <iostream>
#include "core/core.h"

namespace Core
{

class CORE_EXPORT Timer final
{
public:
    using CallbackFunction = std::function<void()>;
    Timer();
    ~Timer();

    template<class Rep, class Period>
    void Start(std::chrono::duration<Rep, Period> interval, CallbackFunction callback);
    template<class Rep, class Period>
    void Start(std::chrono::duration<Rep, Period> interval, size_t repeatCount, CallbackFunction callback);
    void Stop();

    template<class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const
    {
        s << "timer interval = " <<  "total repeat count = " << "repeat cound left = ";
        return s;
    }

protected:
    std::chrono::nanoseconds _interval;
    size_t _initialRepeatCount;
    size_t _currentRepeatCount;
    CallbackFunction _callback;
};

template<class Rep, class Period>
void Timer::Start(std::chrono::duration<Rep, Period> interval, std::function<void()> callback)
{
    _interval = interval;
    _callback = callback;
    _initialRepeatCount = 1;
    _currentRepeatCount = _initialRepeatCount;
}

template<class Rep, class Period>
void Timer::Start(std::chrono::duration<Rep, Period> interval, size_t repeatCount, std::function<void()> callback)
{
    _interval = interval;
    _callback = callback;
    _initialRepeatCount = repeatCount;
    _currentRepeatCount = _initialRepeatCount;
}

template <class Elem, class Traits>
inline void PrintTo(std::basic_ostream<Elem, Traits> & stream, const Timer & value)
{
    value.PrintTo(stream);
}

template <class Elem, class Traits>
inline std::basic_ostream<Elem, Traits> & operator << (std::basic_ostream<Elem, Traits> & stream, const Timer & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace Tools
