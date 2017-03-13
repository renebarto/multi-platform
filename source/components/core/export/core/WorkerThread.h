#pragma once

#include "core/ManualEvent.h"
#include "core/Thread.h"

namespace Core
{

class WorkerThread : public Thread
{
public:
    WorkerThread() = delete;
    WorkerThread(const WorkerThread &) = delete;
    WorkerThread & operator = (const WorkerThread &) = delete;

    explicit WorkerThread(const OSAL::String & name);
    virtual ~WorkerThread();

    virtual void Create();
    void WaitForBirth();
    bool WaitForBirth(int timeout);

    virtual void * Thread() = 0;

    void * GetExitCode();

protected:
    Core::ManualEvent _birthEvent;
    static void StartThread(void *arg);
    void * _exitCode;

    void SetSignalMask();
};

} // namespace Core
