#pragma once

#include <future>
#include <mutex>
#include <thread>
#include <osal/Strings.h>

namespace Core
{

typedef void(* ThreadFunc)(void *);

enum class ThreadState
{
    NotStarted,
    Running,
    Finished,
    Killed,
};

template<typename Result, typename Argument>
class Thread
{
public:
    Thread() = delete;
    Thread(const Thread &) = delete;
    Thread & operator = (const Thread &) = delete;

    Thread(const OSAL::String & name)
        : _name()
        , _threadMutex()
        , _thread()
        , _state(ThreadState::NotStarted)
    {
        SetName(name);
    }

    Thread(const OSAL::String & name, std::function<Result(Argument)> threadFunc, Argument arg)
        : Thread(name)
    {
        Create(threadFunc, arg);
    }

    virtual ~Thread()
    {
        Cleanup();
    }

    void Create(std::function<Result(Argument)> threadFunc, Argument arg)
    {
        Destroy();

//    TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Create thread");

        Lock lock(_threadMutex);
        try
        {
            std::packaged_task<Result(Argument)> task(std::bind(threadFunc, arg));

            _threadResult = task.get_future();
            _thread = std::thread(std::move(task), arg);
            this->_state = ThreadState::Running;
            if (!this->_name.empty())
            {
                pthread_setname_np(_thread.native_handle(), _name.c_str());
            }

//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Thread created");
        }
        catch (const std::exception & e)
        {
            Cleanup();
            throw;
        }
    }

    void Destroy()
    {
        Lock lock(_threadMutex);
        if (IsAlive())
        {
//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Destroy thread");

            WaitForDeath();
            _state = ThreadState::Killed;

//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Thread died");
        }
    }

    bool IsAlive() const
    {
        Lock lock(_threadMutex);
        bool result;

        result = (_thread.native_handle() != 0);

        return result;
    }

    bool IsRunning() const
    {
        Lock lock(_threadMutex);
        bool result;

        result = _thread.native_handle() && (_state == ThreadState::Running);

        return result;
    }

    bool IsFinished() const
    {
        Lock lock(_threadMutex);
        bool result;

        result = (_state == ThreadState::Finished);

        return result;
    }

    bool HasDied() const
    {
        Lock lock(_threadMutex);
        bool result;

        result = (_state == ThreadState::Finished) || (_state == ThreadState::Killed);

        return result;
    }

    OSAL::String GetName() const
    {
        return _name;
    }

    void SetName(const OSAL::String & name)
    {
        _name = name.substr(0, std::min(name.length(), (size_t)15));
    }

    // Wait until _thread is in signaled state (_thread died)
    void WaitForDeath()
    {
        if (!IsAlive())
            return;
        Lock lock(_threadMutex);
        if (_thread.native_handle() != pthread_self())
        {
//        TheLogger().Debug(COMPONENT_NAME, ToString() + ": Wait for thread to die");
            _thread.join();
            _state = ThreadState::Finished;
//        TheLogger().Debug(COMPONENT_NAME, ToString() + ": Thread died");
        }
    }

    Result GetResult()
    {
        // Will thrown exception thrown by thread if it threw,
        // or std::future_error when thread was not run
        return _threadResult.get();
    }
    bool HaveResult()
    {
        return _threadResult.valid();
    }

protected:
    using Mutex = std::recursive_mutex ;
    using Lock = std::lock_guard<Mutex>;

    OSAL::String _name;
    mutable Mutex _threadMutex;
    mutable std::thread _thread;
    std::future<Result> _threadResult;
    ThreadState _state;
    void Cleanup()
    {
//    TheLogger().Debug(COMPONENT_NAME, ToString() + ": Cleanup for thread");

        Destroy();
        Lock lock(_threadMutex);
        _state = ThreadState::NotStarted;
    }
};

} // namespace Core
