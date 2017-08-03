#pragma once

#include "osal/OSAL.h"
WARNING_PUSH
WARNING_DISABLE(4265)
#include <thread>
WARNING_POP
#include "osal/Signal.h"
#include "osal/Strings.h"

namespace OSAL {
namespace Thread {

void SetThreadName(std::thread & thread, const OSAL::String & threadName);
void SetThreadNameSelf(const OSAL::String & threadName);
bool IsThreadSelf(std::thread & thread);
inline bool SetSignalMask(const OSAL::Signal::SignalSet & signalMaskSet)
{
    return (pthread_sigmask(OSAL::Signal::SignalHow::Block, &(signalMaskSet.get()), nullptr) != 0);
}

} // namespace Thread
} // namespace OSAL