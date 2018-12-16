#pragma once

#include <osal/exports.h>
#include <osal/Unused.h>

#include <chrono>
#include <windows.h>

namespace OSAL {
namespace Time {

class Timer
{
public:
    using Callback = std::function<void()>;

    Timer()
        : _callback()
        , _interval()
        , _repeat()
        , _winTimer(INVALID_HANDLE_VALUE)
        , _isExpired()
    {}
    ~Timer()
    {
        if (_winTimer != INVALID_HANDLE_VALUE)
            Stop();
    }

    template<class Rep, class Period>
    bool Start(std::chrono::duration<Rep, Period> interval, Callback callback)
    {
        return Start(interval, std::chrono::duration<Rep, Period>(0), callback);
    }
    template<class Rep, class Period>
    bool StartRepeat(std::chrono::duration<Rep, Period> interval, Callback callback)
    {
        return Start(interval, interval, callback);
    }

    void Stop()
    {
        DeleteTimerQueueTimer(nullptr, _winTimer, INVALID_HANDLE_VALUE);
        _winTimer = INVALID_HANDLE_VALUE;
    }

    bool IsExpired() const { return _isExpired; }

    template<class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const
    {
        s << "timer interval = " <<  _interval << " ns, repeat interval = " << _repeat << " ns,  timed out = " << _isExpired;
        return s;
    }

private:
    static void __stdcall TimerSignalHandler(void * instance, BOOLEAN UNUSED(y))
    {
        Timer * pThis = reinterpret_cast<Timer *>(instance);
        pThis->_isExpired = true;
        pThis->_callback();
    }

    Callback _callback;
    DWORD _interval;
    DWORD _repeat;
    HANDLE _winTimer;
    bool _isExpired;

    template<class Rep, class Period>
    bool Start(std::chrono::duration<Rep, Period> interval, std::chrono::duration<Rep, Period> repeat, Callback callback)
    {
        _callback = callback;

        _interval = static_cast<DWORD>(std::chrono::nanoseconds(interval).count());
        _repeat = static_cast<DWORD>(std::chrono::nanoseconds(repeat).count());
        constexpr DWORD NanoSecondsPerMilliSecond = 1000000;

        if (CreateTimerQueueTimer(&_winTimer, nullptr, (WAITORTIMERCALLBACK)TimerSignalHandler, this,
                                  _interval / NanoSecondsPerMilliSecond, _repeat / NanoSecondsPerMilliSecond, WT_EXECUTEINTIMERTHREAD) == 0)
        {
            return false;
        }

        return true;
    }
};

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

} // namespace Time
} // namespace OSAL