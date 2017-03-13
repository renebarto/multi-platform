#include "core/Thread.h"

#include  <pthread.h>

//#include "core/DefaultLogger.h"
//#include "core/Util.h"

//#ifndef COMPONENT_NAME
//#define COMPONENT_NAME "Core"
//#endif

using namespace std;
using namespace Core;

Thread::Thread(const OSAL::String & name)
    : _name()
    , _threadMutex()
    , _thread()
    , _state(ThreadState::NotStarted)
{
    SetName(name);
}

Thread::Thread(const OSAL::String & name, ThreadFunc threadFunc, void * arg)
    : Thread(name)
{
    CreateThread(threadFunc, arg);
}

Thread::~Thread()
{
    Cleanup();
}

void Thread::CreateThread(ThreadFunc threadFunc, void * arg)
{
    DestroyThread();

//    TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Create thread");

    Lock lock(_threadMutex);
    try
    {
        _thread = std::thread(threadFunc, arg);
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

void Thread::DestroyThread()
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

bool Thread::IsAlive() const
{
    Lock lock(_threadMutex);
    bool result;

    result = _thread.native_handle();

    return result;
}

bool Thread::IsRunning() const
{
    Lock lock(_threadMutex);
    bool result;

    result = _thread.native_handle() && (_state == ThreadState::Running);

    return result;
}

bool Thread::IsFinished() const
{
    Lock lock(_threadMutex);
    bool result;

    result = (_state == ThreadState::Finished);

    return result;
}

bool Thread::HasDied() const
{
    Lock lock(_threadMutex);
    bool result;

    result = (_state == ThreadState::Finished) || (_state == ThreadState::Killed);

    return result;
}

OSAL::String Thread::GetName() const
{
    return _name;
}

void Thread::SetName(const OSAL::String & name)
{
    _name = name.substr(0, min(name.length(), (size_t)15));
}

void Thread::WaitForDeath()
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

void Thread::Cleanup()
{
//    TheLogger().Debug(COMPONENT_NAME, ToString() + ": Cleanup for thread");

    DestroyThread();
    Lock lock(_threadMutex);
    _state = ThreadState::NotStarted;
}
