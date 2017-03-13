#include "core/ActiveObject.h"

//#include "core/DefaultLogger.h"

using namespace std;
using namespace Core;

//#ifndef COMPONENT_NAME
//#define COMPONENT_NAME "Core"
//#endif

ActiveObject::ActiveObject(std::string const & name) :
    WorkerThread(name),
    _isDying(false),
    _isAlive(false),
    _mutex()
{
}

ActiveObject::~ActiveObject()
{
}

// FlushThread must reset all the events
// on which the thread might be waiting.
void ActiveObject::Kill()
{
    if (IsRunning())
    {
        _isDying = true;

//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Signaling thread to shut down");

        FlushThread();

//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Waiting for thread to shut down");

        WaitForDeath();
        DestroyThread();
    }
}

void ActiveObject::Create()
{
    {
        Lock lock(mutex);
        _isDying = false;
    }
    WorkerThread::Create();
}

bool ActiveObject::IsAlive() const
{
    Lock lock(mutex);
    bool result = _isAlive;
    return result;
}

bool ActiveObject::IsDying() const
{
    return _isDying;
}

void * ActiveObject::Thread()
{
    try
    {
        {
            Lock lock(mutex);
            _isAlive = true;
        }

//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Thread starting");

        InitThread();
        Run();
        ExitThread();

//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Thread shutting down");
    }
    catch (const std::exception & e)
    {
//        TheLogger().Debug(COMPONENT_NAME, "Thread " + GetName() + ": Thread threw exception: " + e.what());
    }

    {
        Lock lock(mutex);
        _isAlive = false;
    }
    return nullptr;
}
