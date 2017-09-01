#include "core/AutoEvent.h"

#include <iomanip>
#include <sstream>
#include <osal/OSAL.h>

using namespace std;
using namespace Core;

AutoEvent::AutoEvent() :
    semaphore(),
    isAssigned()
{
    Create();
}

AutoEvent::AutoEvent(const AutoEvent &event):
    semaphore(),
    isAssigned()
{
    Assign(event.semaphore);
}

AutoEvent::AutoEvent(AutoEventHandle handle) :
    semaphore(),
    isAssigned()
{
    Assign(handle);
}

AutoEvent::~AutoEvent()
{
    Destroy();
}

int AutoEvent::GetValue() const
{
    int result;
    if (!OSAL::OS::GetSemaphoreValue(semaphore, result))
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    return result;
}

void AutoEvent::Set()
{
    if (!OSAL::OS::SetSemaphore(semaphore))
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void AutoEvent::Wait()
{
    if (!OSAL::OS::WaitSemaphore(semaphore))
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

bool AutoEvent::Wait(int timeout)
{
    bool timedOut;
    if (!OSAL::OS::WaitSemaphore(semaphore, OSAL::Time::ConvertTimeOffsetToTimeSpec(timeout), timedOut))
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    return !timedOut;
}

OSAL::String AutoEvent::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;
    stream << OSAL::OS::TypeName(*this) << _(" value = ") << GetValue();
    return stream.str();
}

AutoEvent & AutoEvent::operator = (const AutoEvent & other)
{
    Assign(other.semaphore);
    return *this;
}

AutoEvent & AutoEvent::operator = (AutoEventHandle handle)
{
    Assign(handle);
    return *this;
}

AutoEvent::operator AutoEventHandle () const
{
    return semaphore;
}

void AutoEvent::Create()
{
    Destroy();
    isAssigned = false;
    semaphore = OSAL::OS::CreateSemaphore();
    if (semaphore == nullptr)
        OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
}

void AutoEvent::Assign(AutoEventHandle other)
{
    try
    {
        Destroy();
        isAssigned = other != nullptr;
        semaphore = other;
    }
    catch (...)
    {
    }
}

void AutoEvent::Destroy()
{
    if ((semaphore != nullptr) && !isAssigned)
    {
        if (!OSAL::OS::DeleteSemaphore(semaphore))
            OSAL::ThrowOnError(__func__, __FILE__, __LINE__, errno);
    }
    semaphore = nullptr;
}
