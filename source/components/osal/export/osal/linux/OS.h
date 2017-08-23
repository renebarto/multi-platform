#pragma once

#include <semaphore.h>
#include <typeinfo>
#include "osal/Strings.h"

namespace OSAL {
namespace OS {

OSAL_EXPORT String Name();
OSAL_EXPORT String Variant();
OSAL_EXPORT String Release();
OSAL_EXPORT String Version();
OSAL_EXPORT String Platform();

OSAL_EXPORT String DemangleName(const String & mangledName);

template <class T>
inline String TypeName(const T & x)
{
    return DemangleName(typeid(x).name());
}

typedef sem_t * SemaphoreHandle;

inline SemaphoreHandle CreateSemaphore()
{
    SemaphoreHandle semaphore = new sem_t;
    if (sem_init(semaphore, 0, 1))
    {
        delete semaphore;
        return nullptr;
    }
    if (sem_wait(semaphore))
    {
        delete semaphore;
        return nullptr;
    }
    return semaphore;
}

inline bool DeleteSemaphore(SemaphoreHandle semaphore)
{
    if (semaphore != nullptr)
    {
        if (sem_destroy(semaphore) != 0)
            return false;

        delete semaphore;
    }
    return true;
}

inline bool GetSemaphoreValue(SemaphoreHandle semaphore, int & value)
{
    return (sem_getvalue(semaphore, &value) == 0);
}

inline bool SetSemaphore(SemaphoreHandle semaphore)
{
    return (sem_post(semaphore) == 0);
}

inline bool WaitSemaphore(SemaphoreHandle semaphore)
{
    int errorCode = 0;
    errno = 0;
    do
    {
        if (sem_wait(semaphore) != 0)
        {
            errorCode = errno;
        }
    }
    while (errorCode != 0);
    return (errorCode != 0);
}

inline bool WaitSemaphore(SemaphoreHandle semaphore, timespec timeout, bool & timedOut)
{
    timedOut = false;
    timespec time = timeout;
    int errorCode = 0;
    do
    {
        if (sem_timedwait(semaphore, &time) != 0)
        {
            errorCode = errno;
        }
    }
    while ((errorCode != ETIMEDOUT) && (errorCode != 0));
    if (errorCode == ETIMEDOUT)
    {
        timedOut = true;
        return true;
    }
    return (errorCode == 0);
}

} // namespace OS
} // namespace OSAL