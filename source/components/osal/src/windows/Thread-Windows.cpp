#include "osal/Thread.h"

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
    std::string threadNameNarrow = OSAL::ToNarrowString(threadName);
    DWORD threadID = ::GetThreadId(threadHandle);

    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadNameNarrow.c_str();
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
	return static_cast<HANDLE>(thread.native_handle()) == GetCurrentThread();
}

} // namespace Thread
} // namespace OSAL

