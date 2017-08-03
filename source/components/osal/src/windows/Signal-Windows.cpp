#include <osal/Signal.h>

namespace OSAL {
namespace Signal {

static sigset_t signalMask;

int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (oldset != nullptr)
        *oldset = signalMask;
    if (set == nullptr)
        return 0;
    switch (how)
    {
    case SIG_BLOCK:
        signalMask.set |= set->set;
        break;
    case SIG_UNBLOCK:
        signalMask.set &= (~set->set);
        break;
    case SIG_SETMASK:
        signalMask = set;
        break;
    default:
        return -1;
    }
    return 0;
}

}
}
