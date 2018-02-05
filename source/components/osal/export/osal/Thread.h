#pragma once

#include "osal/osal.h"
#include <string>
WARNING_PUSH
WARNING_DISABLE(4265)
#include <thread>
WARNING_POP
#include "osal/Signal.h"

namespace OSAL {
namespace Thread {

OSAL_EXPORT void SetThreadName(std::thread & thread, const std::string & threadName);
OSAL_EXPORT void SetThreadNameSelf(const std::string & threadName);
OSAL_EXPORT bool IsThreadSelf(std::thread & thread);
inline bool SetSignalMask(const OSAL::Signals::SignalSet & signalMaskSet)
{
    return (signalMaskSet.Block() == 0);
}

} // namespace Thread
} // namespace OSAL