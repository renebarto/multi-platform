#include "osal/Thread.h"

#include <pthread.h>
#include <string>
#include "osal/OSAL.h"

using namespace std;
using namespace OSAL;

void OSAL::Thread::SetThreadName(std::thread & thread, const OSAL::String & threadName)
{
    std::string threadNameNarrow = OSAL::ToNarrowString(threadName);

	auto handle = thread.native_handle();
	pthread_setname_np(threadNameNarrow.c_str());
}

void OSAL::Thread::SetThreadNameSelf(const OSAL::String & threadName)
{
    std::string threadNameNarrow = OSAL::ToNarrowString(threadName);
    pthread_setname_np(threadNameNarrow.c_str());
}

bool OSAL::Thread::IsThreadSelf(std::thread & thread)
{
	return thread.native_handle() == pthread_self();
}
