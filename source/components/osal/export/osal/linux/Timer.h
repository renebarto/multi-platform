#pragma once

#include <osal/exports.h>
#include <osal/Assert.h>
#include <osal/Unused.h>

#include <chrono>
#include <functional>
#include <map>
#include <iostream>
#include <time.h>
#include <signal.h>

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
        , _timer()
        , _timerEvent()
        , _timerSpec()
        , _oldHandler()
        , _newHandler()
        , _isExpired()
        , _isSet()
    {
        _timerMap.insert(std::pair<timer_t *, Timer *>(&_timer, this));
    }
    ~Timer()
    {
        _timerMap.erase(&_timer);
        if (_isSet)
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
        _isSet = false;
        timer_delete(_timer);
        sigaction(SIGRTMIN, &_oldHandler, nullptr);
    }

    bool IsExpired() const { return _isExpired; }

    template<class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const
    {
        s << "timer interval = " <<  _interval << " ns, repeat interval = " << _repeat << " ns,  timed out = " << _isExpired;
        return s;
    }

private:
    template<class Rep, class Period>
    bool Start(std::chrono::duration<Rep, Period> interval, std::chrono::duration<Rep, Period> repeat, Callback callback)
    {
        constexpr int64_t NanoSecondsPerSecond = 1000000000;
        _callback = callback;

        _interval = static_cast<size_t>(std::chrono::nanoseconds(interval).count());
        _repeat = static_cast<size_t>(std::chrono::nanoseconds(repeat).count());
        _timerSpec.it_interval.tv_sec = _repeat / NanoSecondsPerSecond;
        _timerSpec.it_interval.tv_nsec = _repeat % NanoSecondsPerSecond;
        _timerSpec.it_value.tv_sec = _interval / NanoSecondsPerSecond;
        _timerSpec.it_value.tv_nsec = _interval % NanoSecondsPerSecond;

        _newHandler.sa_sigaction = TimerSignalHandler;
        _newHandler.sa_flags = SA_SIGINFO;
        sigemptyset(&_newHandler.sa_mask);
        if (sigaction(SIGRTMIN, &_newHandler, &_oldHandler) == -1)
        {
            return false;
        }

        _timerEvent.sigev_notify = SIGEV_SIGNAL;
        _timerEvent.sigev_signo = SIGRTMIN;
        _timerEvent.sigev_value.sival_ptr = &_timer;
        if (timer_create(CLOCK_REALTIME, &_timerEvent, &_timer))
        {
            return false;
        }
        timer_settime(_timer, 0, &_timerSpec, nullptr);

        _isSet = true;

        return true;
    }

    static void TimerSignalHandler(int signal, siginfo_t* signalInfo, void* UNUSED(context))
    {
        ASSERT(signal == SIGRTMIN);
        timer_t * timerInstance = reinterpret_cast<timer_t *>(signalInfo->si_value.sival_ptr);
        if (timerInstance == nullptr)
            return;
        auto entry = _timerMap.find(timerInstance);
        if (entry != _timerMap.end())
        {
            Timer * timer = entry->second;
            timer->_isExpired = true;
            timer->_callback();
        }
    }

    Callback _callback;
    size_t _interval;
    size_t _repeat;
    timer_t _timer;
    sigevent _timerEvent;
    struct itimerspec _timerSpec;
    struct sigaction _oldHandler;
    struct sigaction _newHandler;
    bool _isExpired;
    bool _isSet;
    static std::map<timer_t *, Timer *> _timerMap;
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