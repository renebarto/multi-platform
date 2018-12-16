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
        _callback = callback;

        DWORD milliSeconds = static_cast<DWORD>(std::chrono::milliseconds(interval).count());

        if (CreateTimerQueueTimer(&_winTimer, nullptr, (WAITORTIMERCALLBACK)TimerSignalHandler, this,
                                  milliSeconds, 0, WT_EXECUTEINTIMERTHREAD) == 0)
        {
            return false;
        }

        return true;
    };
    template<class Rep, class Period>
    bool StartRepeat(std::chrono::duration<Rep, Period> interval, Callback callback)
    {
        _callback = callback;

        DWORD milliSeconds = static_cast<DWORD>(std::chrono::milliseconds(interval).count());

        if (CreateTimerQueueTimer(&_winTimer, nullptr, (WAITORTIMERCALLBACK)TimerSignalHandler, this,
                                  milliSeconds, milliSeconds, WT_EXECUTEINTIMERTHREAD) == 0)
        {
            return false;
        }

        return true;
    };

    void Stop()
    {
        DeleteTimerQueueTimer(nullptr, _winTimer, INVALID_HANDLE_VALUE);
        _winTimer = INVALID_HANDLE_VALUE;
    }

    bool IsExpired() const { return _isExpired; }

private:
    static void __stdcall TimerSignalHandler(void * instance, BOOLEAN UNUSED(y))
    {
        Timer * pThis = reinterpret_cast<Timer *>(instance);
        pThis->_isExpired = true;
        pThis->_callback();
    }

    Callback _callback;
    HANDLE _winTimer;
    bool _isExpired;
};

} // namespace Time
} // namespace OSAL