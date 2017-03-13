#pragma once

#include "core/WorkerThread.h"

namespace Core
{

class ActiveObject : protected WorkerThread
{
public:
    ActiveObject() = delete;
    ActiveObject(const ActiveObject &) = delete;
    ActiveObject & operator = (const ActiveObject &) = delete;

    ActiveObject(const OSAL::String & name);
    virtual ~ActiveObject();

    void Kill();
    void Create();

    bool IsAlive() const;
    bool IsDying() const;
    bool HasDied() const { return Thread::HasDied(); };

protected:
    virtual void * Thread() override final;
    virtual void InitThread() {};
    virtual void Run() = 0;
    virtual void ExitThread() {};
    virtual void FlushThread() {};

    volatile bool _isDying;
    bool          _isAlive;
    Mutex         _mutex;
};

} // namespace Core
