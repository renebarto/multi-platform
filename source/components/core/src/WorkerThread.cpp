#include "core/WorkerThread.h"

#include <sstream>
#include <signal.h>
//#include "core/DefaultLogger.h"

//#ifndef COMPONENT_NAME
//#define COMPONENT_NAME "Core"
//#endif

using namespace std;
using namespace Core;

WorkerThread::WorkerThread(const OSAL::String & name)
    : Core::Thread(name)
    , _birthEvent()
    , _exitCode(nullptr)
{
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::Create()
{
    _birthEvent.Reset();
    Thread::CreateThread(StartThread, this);
    WaitForBirth();
}

void WorkerThread::WaitForBirth()
{
    _birthEvent.Wait();
}

bool WorkerThread::WaitForBirth(int timeout)
{
    return _birthEvent.Wait(timeout);
}

void WorkerThread::StartThread(void *arg)
{
    void * result = nullptr;
    try
    {
        WorkerThread * pThis = reinterpret_cast<WorkerThread *>(arg);

        pThis->SetSignalMask();
//        TheLogger().Debug(COMPONENT_NAME, "WorkerThread " + pThis->GetName() + ": Thread starting");
        pThis->_birthEvent.Set();
        result = pThis->Thread();
        pThis->_exitCode = result;
//        TheLogger().Debug(COMPONENT_NAME, "WorkerThread " + pThis->GetName() + ": Thread stopping");
        pThis->_state = ThreadState::Finished;
    }
    catch (const std::exception & e)
    {
//        TheLogger().Debug(COMPONENT_NAME, "WorkerThread " + pThis->GetName() + ": Exception thown: " + e.what());
    }
    catch (...)
    {

    }
}

void WorkerThread::SetSignalMask()
{
    sigset_t signalMaskSet;
    sigemptyset(&signalMaskSet);
    sigaddset(&signalMaskSet, SIGTERM);
    sigaddset(&signalMaskSet, SIGINT);
    sigaddset(&signalMaskSet, SIGQUIT);
    if (pthread_sigmask(SIG_BLOCK, &signalMaskSet, nullptr) != 0)
        throw std::system_error(errno, std::system_category(),
                                  "Cannot set signal mask for thread");
}
