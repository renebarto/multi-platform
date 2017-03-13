#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <sstream>

namespace Core
{

class ManualEvent
{
public:
    ManualEvent()
        : _value(false)
    {
    }
    virtual ~ManualEvent()
    {
    }
    bool GetValue() const { return _value; }

    void Set()
    {
        Lock lock(_mutex);
        _value.store(true);
        _cond.notify_all();
    }
    void Reset()
    {
        Lock lock(_mutex);
        _value.store(false);
    }
    void Wait()
    {
        bool actual = false;
        if (_value.compare_exchange_strong(actual, false))
        {
            Lock lock(_mutex);
            _cond.wait(lock, [&]{return _value.load();});
        }
    }
    bool Wait(int timeoutMS)
    {
        std::chrono::milliseconds timeout(timeoutMS);

        return Wait(std::chrono::duration_cast<std::chrono::nanoseconds>(timeout));
    }
    bool Wait(const std::chrono::nanoseconds & timeout)
    {
        bool result = true;
        bool actual = false;
        if (_value.compare_exchange_strong(actual, false))
        {
            Lock lock(_mutex);
            result = _cond.wait_for(lock, timeout, [&]{return _value.load();});
        }
        return result;
    }

protected:
    std::atomic_bool _value;
    using Mutex = std::mutex;
    using Lock = std::unique_lock<Mutex>;
    Mutex _mutex;
    std::condition_variable _cond;
};

} // namespace Core
