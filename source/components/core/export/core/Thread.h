#pragma once

WARNING_PUSH
WARNING_DISABLE(4265)
WARNING_DISABLE(4355)
#include <future>
#include <mutex>
#include <thread>
#include <osal/Strings.h>
#include <osal/Thread.h>
#include <core/Trace.h>

WARNING_POP

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

    Thread(const std::string & name)
        : _name()
        , _threadMutex()
        , _thread()
        , _state(ThreadState::NotStarted)
    {
        SetName(name);
    }

    Thread(const std::string & name, std::function<Result(Argument)> threadFunc, Argument arg)
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

        TraceDebug("Thread " + GetName() + ": Create thread");

        Lock lock(_threadMutex);
        try
        {
            std::packaged_task<Result(Argument)> task(std::bind(threadFunc, arg));

            _threadResult = task.get_future();
            _thread = std::thread(std::move(task), arg);
            this->_state = ThreadState::Running;
            if (!this->_name.empty())
            {
                OSAL::Thread::SetThreadName(_thread, _name);
            }

            TraceDebug("Thread " + GetName() + ": Thread created");
        }
        catch (const std::exception &)
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
            TraceDebug("Thread " + GetName() + ": Destroy thread");

            WaitForDeath();
            _state = ThreadState::Killed;

            TraceDebug("Thread " + GetName() + ": Thread died");
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

    std::string GetName() const
    {
        return _name;
    }

    void SetName(const std::string & name)
    {
        _name = name.substr(0, std::min(name.length(), (size_t)15));
    }

    // Wait until _thread is in signaled state (_thread died)
    void WaitForDeath()
    {
        if (!IsAlive())
            return;
        Lock lock(_threadMutex);
        if (!OSAL::Thread::IsThreadSelf(_thread))
        {
            TraceDebug(OSAL::System::TypeName(*this) + ": Wait for thread to die");
            _thread.join();
            _state = ThreadState::Finished;
            TraceDebug(OSAL::System::TypeName(*this) + ": Thread died");
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

    std::string _name;
    mutable Mutex _threadMutex;
    mutable std::thread _thread;
    std::future<Result> _threadResult;
    ThreadState _state;
    void Cleanup()
    {
        TraceDebug(OSAL::System::TypeName(*this) + ": Cleanup for thread");

        Destroy();
        Lock lock(_threadMutex);
        _state = ThreadState::NotStarted;
    }
};

} // namespace Core
