#pragma once

#include <mutex>
#include <thread>
#include <osal/strings.h>

namespace Core
{

typedef void (* ThreadFunc)(void * arg);

enum class ThreadState
{
    NotStarted,
    Running,
    Finished,
    Killed,
};

class Thread
{
public:
    Thread() = delete;
    Thread(const Thread &) = delete;
    Thread & operator = (const Thread &) = delete;

    Thread(const OSAL::String & name);
    Thread(const OSAL::String & name, ThreadFunc threadFunc, void * arg);
    virtual ~Thread();

    void CreateThread(ThreadFunc threadFunc, void * arg);
    void DestroyThread();
    bool IsAlive() const;
    bool IsRunning() const;
    bool IsFinished() const;
    bool HasDied() const;
    OSAL::String GetName() const;
    void SetName(const OSAL::String & name);

    // Wait until _thread is in signaled state (_thread died)
    void WaitForDeath();

protected:
    using Mutex = std::recursive_mutex ;
    using Lock = std::lock_guard<Mutex>;

    OSAL::String _name;
    mutable Mutex _threadMutex;
    mutable std::thread _thread;
    ThreadState _state;
    void Cleanup();
};

} // namespace Core
