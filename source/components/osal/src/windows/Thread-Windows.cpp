#include "osal/Thread.h"

#include <iostream>
#include <string>
#include "osal/OSAL.h"

using namespace std;

namespace OSAL {
namespace Thread {

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

static void RaiseException(THREADNAME_INFO & info)
{
#pragma warning(push)
#pragma warning(disable: 6320 6322)
    __try
    {
        ::RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
#pragma warning(pop)
}

static void SetThreadName(HANDLE threadHandle, const std::string & threadName)
{
    DWORD threadID = ::GetThreadId(threadHandle);

    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName.c_str();
    info.dwThreadID = threadID;
    info.dwFlags = 0;

    RaiseException(info);
}

void OSAL::Thread::SetThreadName(std::thread & thread, const std::string & threadName)
{
    SetThreadName(static_cast<HANDLE>(thread.native_handle()), threadName);
}

void OSAL::Thread::SetThreadNameSelf(const std::string & threadName)
{
    SetThreadName(GetCurrentThread(), threadName);
}

bool OSAL::Thread::IsThreadSelf(std::thread & thread)
{
    // This is probably portable
    return thread.get_id() == std::this_thread::get_id();
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

