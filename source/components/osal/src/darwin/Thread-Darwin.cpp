#include "osal/Thread.h"

#include "osal/osal.h"
#include <pthread.h>
#include <string>

using namespace std;

namespace OSAL {
namespace Thread {

void SetThreadName(thread & thread, const string & threadName)
{
    auto handle = thread.native_handle();
    pthread_setname_np(threadName.c_str());
}

void SetThreadNameSelf(const string & threadName)
{
    pthread_setname_np(threadName.c_str());
}

bool IsThreadSelf(thread & thread)
{
    return thread.native_handle() == pthread_self();
}

} // namespace Thread
} // namespace OSAL
