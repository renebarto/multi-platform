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
    int policy;
    struct sched_param sched;
    int result = pthread_getschedparam(pthread_self(), &policy, &sched);
    if (result != 0)
        throw std::runtime_error("Failed to get the current thread priority!");
    if ((policy == SCHED_FIFO) || (policy == SCHED_RR))
    {
        if (sched.sched_priority < 15)
            return Priority::IDLE;
        else if (sched.sched_priority < 30)
            return Priority::LOWEST;
        else if (sched.sched_priority < 50)
            return Priority::LOW;
        else if (sched.sched_priority < 70)
            return Priority::NORMAL;
        else if (sched.sched_priority < 85)
            return Priority::HIGH;
        else if (sched.sched_priority < 99)
            return Priority::HIGHEST;
        else
            return Priority::REALTIME;
    } else
        return Priority::NORMAL;
}

Priority GetPriority(std::thread& thread)
{
    int policy;
    struct sched_param sched;
    int result = pthread_getschedparam(thread.native_handle(), &policy, &sched);
    if (result != 0)
        throw std::runtime_error("Failed to get the given thread priority!");
    if ((policy == SCHED_FIFO) || (policy == SCHED_RR))
    {
        if (sched.sched_priority < 15)
            return Priority::IDLE;
        else if (sched.sched_priority < 30)
            return Priority::LOWEST;
        else if (sched.sched_priority < 50)
            return Priority::LOW;
        else if (sched.sched_priority < 70)
            return Priority::NORMAL;
        else if (sched.sched_priority < 85)
            return Priority::HIGH;
        else if (sched.sched_priority < 99)
            return Priority::HIGHEST;
        else
            return Priority::REALTIME;
    } else
        return Priority::NORMAL;
}

void SetPriority(Priority priority)
{
    int policy = SCHED_RR;
    struct sched_param sched;
    sched.sched_priority = 50;
    switch (priority)
    {
        case Priority::IDLE:
            sched.sched_priority = 1;
            break;
        case Priority::LOWEST:
            sched.sched_priority = 15;
            break;
        case Priority::LOW:
            sched.sched_priority = 30;
            break;
        case Priority::NORMAL:
            sched.sched_priority = 50;
            break;
        case Priority::HIGH:
            sched.sched_priority = 70;
            break;
        case Priority::HIGHEST:
            sched.sched_priority = 85;
            break;
        case Priority::REALTIME:
            sched.sched_priority = 99;
            break;
    }

    int result = pthread_setschedparam(pthread_self(), policy, &sched);
    if (result != 0)
        throw std::runtime_error("Failed to set the current thread priority!");
}

void SetPriority(std::thread& thread, Priority priority)
{
    int policy = SCHED_RR;
    struct sched_param sched;
    sched.sched_priority = 50;
    switch (priority)
    {
        case Priority::IDLE:
            sched.sched_priority = 1;
            break;
        case Priority::LOWEST:
            sched.sched_priority = 15;
            break;
        case Priority::LOW:
            sched.sched_priority = 30;
            break;
        case Priority::NORMAL:
            sched.sched_priority = 50;
            break;
        case Priority::HIGH:
            sched.sched_priority = 70;
            break;
        case Priority::HIGHEST:
            sched.sched_priority = 85;
            break;
        case Priority::REALTIME:
            sched.sched_priority = 99;
            break;
    }

    int result = pthread_setschedparam(thread.native_handle(), policy, &sched);
    if (result != 0)
        throw std::runtime_error("Failed to set the given thread priority!");
}

} // namespace Thread
} // namespace OSAL

