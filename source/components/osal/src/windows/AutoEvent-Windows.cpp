#include "osal/AutoEvent.h"

#include <iostream>
#include <iomanip>
#include <sstream>
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
    if (Wait(std::chrono::nanoseconds(0)))
    {
        Set();
        return true;
    }
    return false;
}

void AutoEvent::Set()
{
    if (SetEvent(_semaphore) != 0)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, static_cast<int>(GetLastError()));
}

void AutoEvent::Wait()
{
    int errorCode = 0;
    do
    {
        DWORD waitResult = WaitForSingleObject(_semaphore, INFINITE);
        if (waitResult == WAIT_FAILED)
        {
            errorCode = static_cast<int>(GetLastError());
        }
        else
            errorCode = 0;
    }
    while (errorCode != 0);
    OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errorCode);
}

bool AutoEvent::Wait(const std::chrono::nanoseconds & timeout)
{
    DWORD timeMS = static_cast<DWORD>(timeout.count() / 1000000);
    int errorCode = 0;
    do
    {
        DWORD waitResult = WaitForSingleObject(_semaphore, timeMS);
        if (waitResult == WAIT_FAILED)
        {
            errorCode = static_cast<int>(GetLastError());
        }
        else if (waitResult == WAIT_TIMEOUT)
            errorCode = ETIMEDOUT;
        else
            errorCode = 0;
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
    Assign(other._semaphore);
    return *this;
}

AutoEvent & AutoEvent::operator = (AutoEventHandle handle)
{
    Assign(handle);
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
    _semaphore = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (_semaphore == nullptr)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, static_cast<int>(GetLastError()));
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
        if (CloseHandle(_semaphore) != 0)
            OSAL::ThrowOnError(__func__, __FILE__, __LINE__, static_cast<int>(GetLastError()));
        _semaphore = nullptr;
    }
    _semaphore = 0;
}
