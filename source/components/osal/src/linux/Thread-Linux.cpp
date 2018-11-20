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

} // namespace Thread
} // namespace OSAL

