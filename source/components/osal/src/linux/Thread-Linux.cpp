#include "osal/Thread.h"

#include <pthread.h>
#include <string>
#include "osal/OSAL.h"

using namespace std;

namespace OSAL
{
namespace Thread
{

static void SetThreadName(pthread_t threadID, const OSAL::String & threadName)
{
    std::string threadNameNarrow = OSAL::ToNarrowString(threadName);

    pthread_setname_np(threadID, threadNameNarrow.c_str());
}

void SetThreadName(std::thread & thread, const OSAL::String & threadName)
{
    SetThreadName(thread.native_handle(), threadName);
}

void SetThreadNameSelf(const OSAL::String & threadName)
{
    SetThreadName(pthread_self(), threadName);
}

bool IsThreadSelf(std::thread & thread)
{
    return thread.native_handle() == pthread_self();
}

} // namespace Thread
} // namespace OSAL

