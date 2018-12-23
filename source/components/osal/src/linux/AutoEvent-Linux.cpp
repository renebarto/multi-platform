#include "osal/AutoEvent.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <semaphore.h>
#include "osal/Time.h"
#include "osal/Exception.h"

using namespace std;
using namespace OSAL;

AutoEvent::AutoEvent()
    : _semaphore(nullptr)
    , _isAssigned(false)
{
    Create();
}

AutoEvent::AutoEvent(const AutoEvent &event)
    : _semaphore(nullptr)
    , _isAssigned(false)
{
    Assign(event._semaphore);
}

AutoEvent::AutoEvent(AutoEventHandle handle)
    : _semaphore(nullptr)
    , _isAssigned(false)
{
    Assign(handle);
}

AutoEvent::~AutoEvent()
{
    Destroy();
}

bool AutoEvent::Get()
{
    int result;
    if (sem_getvalue(reinterpret_cast<sem_t *>(_semaphore), &result) != 0)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    return result != 0;
}

void AutoEvent::Set()
{
    if (sem_post(reinterpret_cast<sem_t *>(_semaphore)) != 0)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void AutoEvent::Wait()
{
    int errorCode = 0;
    do
    {
        if (sem_wait(reinterpret_cast<sem_t *>(_semaphore)) != 0)
        {
            errorCode = errno;
        }
    }
    while (errorCode != 0);
    OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
}

bool AutoEvent::Wait(const std::chrono::nanoseconds & timeout)
{
    OSAL::Time::timespec time;
    time.tv_nsec = timeout.count() % 1000000000;
    time.tv_sec = timeout.count() / 1000000000;
    int errorCode = 0;
    do
    {
        if (sem_timedwait(reinterpret_cast<sem_t *>(_semaphore), &time) != 0)
        {
            errorCode = errno;
        }
    }
    while ((errorCode != ETIMEDOUT) && (errorCode != 0));
    if (errorCode == ETIMEDOUT)
    {
        return false;
    }
    OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
    return true;
}

AutoEvent & AutoEvent::operator = (const AutoEvent & other)
{
    Assign(reinterpret_cast<sem_t *>(other._semaphore));
    return *this;
}

AutoEvent & AutoEvent::operator = (AutoEventHandle handle)
{
    Assign(reinterpret_cast<sem_t *>(handle));
    return *this;
}

AutoEvent::operator AutoEventHandle () const
{
    return _semaphore;
}

bool OSAL::operator == (AutoEventHandle handle, AutoEvent & event)
{
    return event._semaphore == handle;
}

bool OSAL::operator == (AutoEvent & event, AutoEventHandle handle)
{
    return event._semaphore == handle;
}

bool OSAL::operator != (AutoEventHandle handle, AutoEvent &event)
{
    return event._semaphore != handle;
}

bool OSAL::operator != (AutoEvent &event, AutoEventHandle handle)
{
    return event._semaphore != handle;
}

void AutoEvent::Create()
{
    Destroy();
    _isAssigned = false;
    _semaphore = AutoEventHandle(new sem_t);
    if (sem_init(reinterpret_cast<sem_t *>(_semaphore), 0, 1))
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    if (sem_wait(reinterpret_cast<sem_t *>(_semaphore)))
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void AutoEvent::Assign(AutoEventHandle other)
{
    try
    {
        Destroy();
        _isAssigned = other != 0;
        _semaphore = other;
    }
    catch (...)
    {
    }
}

void AutoEvent::Destroy()
{
    if ((_semaphore != 0) && !_isAssigned)
    {
        if (sem_destroy(reinterpret_cast<sem_t *>(_semaphore)) != 0)
            OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);

        delete reinterpret_cast<sem_t *>(_semaphore);
    }
    _semaphore = 0;
}
