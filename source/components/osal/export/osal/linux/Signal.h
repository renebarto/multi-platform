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

class SignalSet
{
public:
    SignalSet()
            : _set()
    {
        clear();
    }
    int clear()
    {
        return sigemptyset(&_set);
    }
    int fill()
    {
        return sigfillset(&_set);
    }
    int add(int signum)
    {
        return sigaddset(&_set, signum);
    }
    int remove(int signum)
    {
        return sigdelset(&_set, signum);
    }
    int contains(int signum)
    {
        return sigismember(&_set, signum);
    }
    const sigset_t & get() const
    {
        return _set;
    }

private:
    sigset_t _set;
};

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