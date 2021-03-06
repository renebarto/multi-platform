#include <osal/Signal.h>

namespace OSAL {
namespace Signal {

static sigset_t signalMask;

int SetSignalMask(SignalHow how, const sigset_t *set, sigset_t *oldset)
{
    if (oldset != nullptr)
        *oldset = signalMask;
    if (set == nullptr)
        return 0;
    switch (how)
    {
    case SignalHow::Block:
        signalMask.set |= set->set;
        break;
    case SignalHow::Unblock:
        signalMask.set &= (~set->set);
        break;
    case SignalHow::SetMask:
        signalMask = *set;
        break;
    default:
        return -1;
    }
    return 0;
}

} // namespace Signal
} // namespace OSAL
