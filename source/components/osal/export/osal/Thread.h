#pragma once

#include "osal/OSAL.h"
WARNING_PUSH
WARNING_DISABLE(4265)
#include <thread>
WARNING_POP
#include "osal/Strings.h"

namespace OSAL {
namespace Thread {

void SetThreadName(std::thread & thread, const OSAL::String & threadName);
bool IsThreadSelf(std::thread & thread);

} // namespace Thread
} // namespace OSAL