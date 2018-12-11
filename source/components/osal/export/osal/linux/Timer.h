#pragma once

#include <osal/exports.h>
#include <osal/Unused.h>

#include <chrono>
#include <sys/time.h>
#include <signal.h>

namespace OSAL {
namespace Time {

class Timer
{
public:
    using Callback = std::function<void()>;

    Timer()
        : _callback()
        , _timerValue()
        , _oldHandler()
        , _newHandler()
        , _isExpired()
    {}

    template<class Rep, class Period>
    bool StartTimer(std::chrono::duration<Rep, Period> interval, Callback callback)
    {
        constexpr int64_t MicroSecondsPerSecond = 1000000;
        _callback = callback;

        size_t microSeconds = std::chrono::microseconds(interval).count();
        _timerValue.it_interval.tv_sec = microSeconds;
        _timerValue.it_interval.tv_usec = (microSeconds % MicroSecondsPerSecond);
        _timerValue.it_value.tv_sec = _timerValue.it_interval.tv_sec;
        _timerValue.it_value.tv_usec = _timerValue.it_interval.tv_usec;
        if (setitimer(ITIMER_REAL, &_timerValue, nullptr))
        {
            return false;
        }

        _newHandler.sa_handler = &TimerSignalHandler;
        _newHandler.sa_flags = SA_NOMASK;
        if (sigaction(SIGALRM, &_newHandler, &_oldHandler))
        {
            return false;
        }

        return true;
    };

    void StopTimer()
    {
        _timerValue.it_interval.tv_sec = 0;
        _timerValue.it_interval.tv_usec = 0;
        _timerValue.it_value.tv_sec = 0;
        _timerValue.it_value.tv_usec = 0;
        setitimer(ITIMER_REAL, &_timerValue, nullptr);

        sigaction(SIGALRM, &_oldHandler, nullptr);
    }

    bool IsExpired() const { return _isExpired; }

private:
    void TimerSignalHandler(int UNUSED(signal))
    {
        _isExpired = true;
        _callback();
    }

    Callback _callback;
    struct itimerval _timerValue;
    struct sigaction _oldHandler;
    struct sigaction _newHandler;
    bool _isExpired;
};

} // namespace Time
} // namespace OSAL