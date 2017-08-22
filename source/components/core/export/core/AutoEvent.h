#pragma once

#include <memory>
#include <osal/Strings.h>
#include <osal/OS.h>

namespace Core
{

typedef OSAL::OS::SemaphoreHandle AutoEventHandle;

class AutoEvent
{
public:
    typedef std::shared_ptr<AutoEvent> Ptr;

    AutoEvent();
    AutoEvent(const AutoEvent &event);
    AutoEvent(AutoEventHandle handle);
    virtual ~AutoEvent();
    int GetValue() const;
    void Set();
    void Wait();
    bool Wait(int timeout);

    virtual OSAL::String ToString() const;

    AutoEvent &operator = (const AutoEvent & other);
    AutoEvent &operator = (AutoEventHandle handle);
    operator AutoEventHandle () const;
    friend bool operator == (AutoEventHandle handle, AutoEvent & event);
    friend bool operator == (AutoEvent & event, AutoEventHandle handle);
    friend bool operator != (AutoEventHandle handle, AutoEvent & event);
    friend bool operator != (AutoEvent & event, AutoEventHandle handle);

protected:
    AutoEventHandle semaphore;
    bool isAssigned;
    void Create();
    void Assign(AutoEventHandle other);
    void Destroy();
};

inline bool operator == (AutoEventHandle handle, AutoEvent & event)
{
    return event.semaphore == handle;
}

inline bool operator == (AutoEvent & event, AutoEventHandle handle)
{
    return event.semaphore == handle;
}

inline bool operator != (AutoEventHandle handle, AutoEvent &event)
{
    return event.semaphore != handle;
}

inline bool operator != (AutoEvent &event, AutoEventHandle handle)
{
    return event.semaphore != handle;
}

inline void PrintTo(const AutoEvent & value, std::ostream & stream)
{
    stream << value.ToString();
}

} // namespace Core
