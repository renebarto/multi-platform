#pragma once

#include <chrono>
#include "osal/Signal.h"
#include "core/ManualEvent.h"
#include "core/Thread.h"

namespace Core
{

template<typename Result>
class WorkerThread : public Thread<Result, WorkerThread<Result> *>
{
public:
    WorkerThread() = delete;
    WorkerThread(const WorkerThread &) = delete;
    WorkerThread & operator = (const WorkerThread &) = delete;

    explicit WorkerThread(const OSAL::String & name)
        : Core::Thread<Result, WorkerThread<Result> *>(name)
        , _birthEvent()
    {
    }
    virtual ~WorkerThread()
    {
    }

    virtual void Create()
    {
        _birthEvent.Reset();
        Core::Thread<Result, WorkerThread<Result> *>::Create(ThreadStarter, this);
        WaitForBirth();
    }
    void WaitForBirth()
    {
        _birthEvent.Wait();
    }
    bool WaitForBirth(int timeoutMS)
    {
        return _birthEvent.Wait(timeoutMS);
    }
    bool WaitForBirth(std::chrono::nanoseconds timeout)
    {
        return _birthEvent.Wait(timeout);
    }

    virtual Result Thread() = 0;

protected:
    Core::ManualEvent _birthEvent;
    static Result ThreadStarter(WorkerThread * arg)
    {
        WorkerThread * pThis = reinterpret_cast<WorkerThread *>(arg);

        Result result {};
        try
        {

            pThis->SetSignalMask();
            TheLogger().Debug(ComponentName, _("WorkerThread ") + pThis->GetName() + _(": Thread starting"));
            pThis->_birthEvent.Set();
            result = pThis->Thread();
            TheLogger().Debug(ComponentName, _("WorkerThread ") + pThis->GetName() + _(": Thread stopping"));
            pThis->_state = ThreadState::Finished;
        }
        catch (const std::exception & e)
        {
            TheLogger().Debug(ComponentName, _("WorkerThread ") + pThis->GetName() + _(": Exception thown: ") + OSAL::ToString(e.what()));
            throw;
        }
        return result;
    }

    void SetSignalMask()
    {
        OSAL::Signal::sigset_t signalMaskSet;
        OSAL::Signal::sigemptyset(&signalMaskSet);
        OSAL::Signal::sigaddset(&signalMaskSet, SIGTERM);
        OSAL::Signal::sigaddset(&signalMaskSet, SIGINT);
#if defined(SIGQUIT)
        OSAL::Signal::sigaddset(&signalMaskSet, SIGQUIT);
#endif
        if (pthread_sigmask(OSAL::Signal::Block, &signalMaskSet, nullptr) != 0)
            throw OSAL::SystemError(__func__, __FILE__, __LINE__, errno,
                                    _("Cannot set signal mask for thread"));
    }
};

} // namespace Core
