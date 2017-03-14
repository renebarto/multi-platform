#pragma once

#include "core/WorkerThread.h"

namespace Core
{

template<typename Result>
class ActiveObject : public WorkerThread<Result>
{
public:
    ActiveObject() = delete;
    ActiveObject(const ActiveObject &) = delete;
    ActiveObject & operator = (const ActiveObject &) = delete;

    ActiveObject(const OSAL::String & name)
        : WorkerThread<Result>(name)
        , _isDying(false)
        , _isAlive(false)
        , _mutex()
    {
    }
    virtual ~ActiveObject()
    {
    }

    void Kill()
    {
        if (WorkerThread<Result>::IsRunning())
        {
            _isDying = true;

//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Signaling thread to shut down");

            FlushThread();

//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Waiting for thread to shut down");

            WorkerThread<Result>::WaitForDeath();
            WorkerThread<Result>::Destroy();
        }
    }
    void Create()
    {
        {
            Lock lock(_mutex);
            _isDying = false;
        }
        WorkerThread<Result>::Create();
    }

    bool IsAlive() const
    {
        Lock lock(_mutex);
        bool result = _isAlive;
        return result;
    }
    bool IsDying() const
    {
        return _isDying;
    }

protected:
    virtual Result Thread() override final
    {
        Result result {};
        try
        {
            {
                Lock lock(_mutex);
                _isAlive = true;
            }

//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Thread starting");

            InitThread();
            result = Run();
            ExitThread();

//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Thread shutting down");
        }
        catch (const std::exception & e)
        {
//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Thread threw exception: " + e.what());
            Lock lock(_mutex);
            _isAlive = false;
            throw;
        }

        {
            Lock lock(_mutex);
            _isAlive = false;
        }
        return result;
    }
    virtual void InitThread() {};
    virtual Result Run() = 0;
    virtual void ExitThread() {};
    virtual void FlushThread() {};

    using Mutex = std::recursive_mutex ;
    using Lock = std::lock_guard<Mutex>;
    volatile bool _isDying;
    bool          _isAlive;
    Mutex         _mutex;
};

} // namespace Core
