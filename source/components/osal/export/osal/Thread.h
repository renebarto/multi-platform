#pragma once

#include "osal/osal.h"
WARNING_PUSH
WARNING_DISABLE(4265)
#include <string>
#include <thread>
WARNING_POP
#include "osal/Signal.h"

namespace OSAL {
namespace Thread {

OSAL_EXPORT void SetThreadName(std::thread & thread, const std::string & threadName);
OSAL_EXPORT void SetThreadNameSelf(const std::string & threadName);
OSAL_EXPORT bool IsThreadSelf(std::thread & thread);
inline bool SetSignalMask(const OSAL::Signal::SignalSet & signalMaskSet)
{
    return (SetSignalMask(OSAL::Signal::SignalHow::Block, &(signalMaskSet.get()), nullptr) == 0);
}

} // namespace Thread
} // namespace OSAL