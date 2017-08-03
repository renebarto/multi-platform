#pragma once

#include <signal.h>

namespace OSAL {
namespace Signal {

typedef struct ::sigaction sigaction_t;

inline int sigaction(int signum, const sigaction_t *act, sigaction_t *oldact)
{
    return ::sigaction(signum, act, oldact);
}

using sighandler_t = ::sighandler_t;

inline sighandler_t signal(int signum, sighandler_t handler)
{
    return ::signal(signum, handler);
}

using sigset_t = ::sigset_t;

inline int sigemptyset(sigset_t *set)
{
    return ::sigemptyset(set);
}

inline int sigfillset(sigset_t *set)
{
    return ::sigfillset(set);
}

inline int sigaddset(sigset_t *set, int signum)
{
    return ::sigaddset(set, signum);
}

inline int sigdelset(sigset_t *set, int signum)
{
    return ::sigdelset(set, signum);
}

inline int sigismember(const sigset_t *set, int signum)
{
    return ::sigismember(set, signum);
}

enum SignalHow
{
    Block = SIG_BLOCK,
    Unblock = SIG_UNBLOCK,
    SetMask = SIG_SETMASK,
};

inline int pthread_sigmask(SignalHow how, const sigset_t *set, sigset_t *oldset)
{
    return ::pthread_sigmask(how, set, oldset);
}

} // namespace Signal
} // namespace OSAL