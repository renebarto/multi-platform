#pragma once

#include <signal.h>
WARNING_PUSH
WARNING_DISABLE(4265)
#include <bitset>
WARNING_POP

namespace OSAL {
namespace Signal {

union sigval
{
    int    sival_int;       // Integer signal value.
    void  *sival_ptr;       // Pointer signal value.
};

using pid_t = int;
using uid_t = int;

struct siginfo_t
{
    int           si_signo; // Signal number.
    int           si_errno; // If non - zero, an errno value associated with this signal, as defined in <errno.h>.

    int           si_code;  // Signal code.

    pid_t         si_pid;   // Sending process ID.
    uid_t         si_uid;   // Real user ID of sending process.
    void         *si_addr;  // Address of faulting instruction.
    int           si_status;// Exit value or signal.
    long          si_band;  // Band event for SIGPOLL.

    union sigval  si_value; // Signal value.
};

struct sigset_t
{
    std::bitset<320> set;
};

struct sigaction_t
{
    void(*sa_handler)(int);
    void(*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t   sa_mask;
    int        sa_flags;
    void(*sa_restorer)(void);
};

inline int sigaction(int signum, const sigaction_t *act, sigaction_t *oldact)
{
    return -1;
}

using sighandler_t = ::_crt_signal_t;

inline sighandler_t signal(int signum, sighandler_t handler)
{
    return ::signal(signum, handler);
}

inline int sigemptyset(sigset_t *set)
{
    if (set == nullptr)
        return -1;
    set->set.reset();
    return 0;
}

inline int sigfillset(sigset_t *set)
{
    if (set == nullptr)
        return -1;
    set->set.set();
    return 0;
}

inline int sigaddset(sigset_t *set, int signum)
{
    if (set == nullptr)
        return -1;
    if ((signum < 0) || (static_cast<size_t>(signum) >= set->set.size()))
        return -1;
    set->set.set(static_cast<size_t>(signum));
    return 0;
}

inline int sigdelset(sigset_t *set, int signum)
{
    if (set == nullptr)
        return -1;
    if ((signum < 0) || (static_cast<size_t>(signum) >= set->set.size()))
        return -1;
    set->set.reset(static_cast<size_t>(signum));
    return 0;
}

inline int sigismember(const sigset_t *set, int signum)
{
    if (set == nullptr)
        return -1;
    if ((signum < 0) || (static_cast<size_t>(signum) >= set->set.size()))
        return -1;
    return (set->set.test(static_cast<size_t>(signum))) ? 1 : 0;
}

enum SignalHow
{
    Block = 0,
    Unblock = 1,
    SetMask = 2,
};

OSAL_EXPORT int pthread_sigmask(SignalHow how, const sigset_t *set, sigset_t *oldset);

} // namespace Signal
} // namespace OSAL