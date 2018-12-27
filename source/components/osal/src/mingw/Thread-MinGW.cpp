#include "osal/Thread.h"

#include <pthread.h>
#include <string>
#include "osal/osal.h"

using namespace std;

namespace OSAL
{
namespace Thread
{

static void SetThreadName(pthread_t threadID, const std::string & threadName)
{
    pthread_setname_np(threadID, threadName.c_str());
}

void SetThreadName(std::thread & thread, const std::string & threadName)
{
    SetThreadName(thread.native_handle(), threadName);
}

void SetThreadNameSelf(const std::string & threadName)
{
    SetThreadName(pthread_self(), threadName);
}

bool IsThreadSelf(std::thread & thread)
{
    return thread.native_handle() == pthread_self();
}


Priority GetPriority()
{
    int priority = GetThreadPriority(GetCurrentThread());
    if (priority == THREAD_PRIORITY_ERROR_RETURN)
        throw std::runtime_error("Failed to get the current thread priority!");
    if (priority < THREAD_PRIORITY_LOWEST)
        return Priority::IDLE;
    else if (priority < THREAD_PRIORITY_BELOW_NORMAL)
        return Priority::LOWEST;
    else if (priority < THREAD_PRIORITY_NORMAL)
        return Priority::LOW;
    else if (priority < THREAD_PRIORITY_ABOVE_NORMAL)
        return Priority::NORMAL;
    else if (priority < THREAD_PRIORITY_HIGHEST)
        return Priority::HIGH;
    else if (priority < THREAD_PRIORITY_TIME_CRITICAL)
        return Priority::HIGHEST;
    else
        return Priority::REALTIME;
}

Priority GetPriority(std::thread& thread)
{
    int priority = GetThreadPriority((HANDLE)thread.native_handle());
    if (priority == THREAD_PRIORITY_ERROR_RETURN)
        throw std::runtime_error("Failed to get the given thread priority!");
    if (priority < THREAD_PRIORITY_LOWEST)
        return Priority::IDLE;
    else if (priority < THREAD_PRIORITY_BELOW_NORMAL)
        return Priority::LOWEST;
    else if (priority < THREAD_PRIORITY_NORMAL)
        return Priority::LOW;
    else if (priority < THREAD_PRIORITY_ABOVE_NORMAL)
        return Priority::NORMAL;
    else if (priority < THREAD_PRIORITY_HIGHEST)
        return Priority::HIGH;
    else if (priority < THREAD_PRIORITY_TIME_CRITICAL)
        return Priority::HIGHEST;
    else
        return Priority::REALTIME;
}

void SetPriority(Priority priority)
{
    int nPriority = THREAD_PRIORITY_NORMAL;
    switch (priority)
    {
        case Priority::IDLE:
            nPriority = THREAD_PRIORITY_IDLE;
            break;
        case Priority::LOWEST:
            nPriority = THREAD_PRIORITY_LOWEST;
            break;
        case Priority::LOW:
            nPriority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case Priority::NORMAL:
            nPriority = THREAD_PRIORITY_NORMAL;
            break;
        case Priority::HIGH:
            nPriority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case Priority::HIGHEST:
            nPriority = THREAD_PRIORITY_HIGHEST;
            break;
        case Priority::REALTIME:
            nPriority = THREAD_PRIORITY_TIME_CRITICAL;
            break;
    }

    if (!SetThreadPriority(GetCurrentThread(), nPriority))
        throw std::runtime_error("Failed to set the current thread priority!");
}

void SetPriority(std::thread& thread, Priority priority)
{
    int nPriority = THREAD_PRIORITY_NORMAL;
    switch (priority)
    {
        case Priority::IDLE:
            nPriority = THREAD_PRIORITY_IDLE;
            break;
        case Priority::LOWEST:
            nPriority = THREAD_PRIORITY_LOWEST;
            break;
        case Priority::LOW:
            nPriority = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case Priority::NORMAL:
            nPriority = THREAD_PRIORITY_NORMAL;
            break;
        case Priority::HIGH:
            nPriority = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case Priority::HIGHEST:
            nPriority = THREAD_PRIORITY_HIGHEST;
            break;
        case Priority::REALTIME:
            nPriority = THREAD_PRIORITY_TIME_CRITICAL;
            break;
    }

    if (!SetThreadPriority((HANDLE)thread.native_handle(), nPriority))
        throw std::runtime_error("Failed to set the given thread priority!");
}

} // namespace Thread
} // namespace OSAL

