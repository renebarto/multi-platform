#pragma once

#include <signal.h>

namespace OSAL {
namespace Signal {

enum class Signal
{
    SigHangup = SIGHUP,                 // Term    Hangup detected on controlling terminal or death of controlling process
    SigInterrupt = SIGINT,              // Term    Interrupt from keyboard (ANSI)
    SigQuit = SIGQUIT,                  // Core    Quit from keyboard (POSIX)
    SigIllegalInstruction = SIGILL,     // Core    Illegal Instruction (ANSI)
    SigTrap = SIGTRAP,                  // Core    Trace Trap (POSIX)
    SigAbort = SIGABRT,                 // Core    Abort signal from abort(3) (ANSI) / IOT trap (4.2 BSD)
    SigBus = SIGBUS,                    // Core    BUS error (4.2 BSD)
    SigFloatingPoint = SIGFPE,          // Core    Floating-point exception (ANSI)
    SigKill = SIGKILL,                  // Term    Kill signal (POSIX)
    SigUser1 = SIGUSR1,                 // Core    User-defined signal 1 (POSIX)
    SigSegmentViolation = SIGSEGV,      // Core    Invalid memory reference / Segment violation (POSIX)
    SigUser2 = SIGUSR2,                 // Core    User-defined signal 2 (POSIX)
    SigPipe = SIGPIPE,                  // Term    Broken pipe: write to pipe with no readers; see pipe(7) (POSIX)
    SigAlarm = SIGALRM,                 // Term    Timer signal from alarm(2) (POSIX)
    SigTerminate = SIGTERM,             // Term    Termination signal (ANSI)
    SigStackFault = SIGSTKFLT,          // Core    Stack fault
    SigChild = SIGCHLD,                 // Ign     Child stopped or terminated (POSIX)
    SigContinue = SIGCONT,              // Cont    Continue if stopped (POSIX)
    SigStop = SIGSTOP,                  // Stop    Stop process, unblockable (POSIX)
    SigTerminalStop = SIGTSTP,          // Stop    Stop typed at terminal (POSIX)
    SigTTYIn = SIGTTIN,                 // Stop    Terminal input for background process (POSIX)
    SigTTYOut = SIGTTOU,                // Stop    Terminal output for background process (POSIX)
    SigUrgent = SIGURG,                 //         Urgent condition on socket (4.2 BSD)
    SigCPULimit = SIGXCPU,              //         CPU limit exceeded (4.2 BSD)
    SigFileSizeLimit = SIGXFSZ,         //         File size limit exceeded (4.2 BSD)
    SigVirtualAlarm = SIGVTALRM,        //         Virtual alarm clock (4.2 BSD)
    SigProfileAlarm = SIGPROF,          //         Profiling alarm clock (4.2 BSD)
    SigWindowSize = SIGWINCH,           //         Window size change (4.3 BSD, Sun)
    SigPoll = SIGPOLL,                  //         Pollable event occurred (System V)
    SigIO = SIGIO,                      //         I/O now possible (4.2 BSD)
    SigPower = SIGPWR,                  //         Power failure restart (System V)
    SigSystemCall = SIGSYS,             //         Bad system call
    SigUnused = SIGUNUSED,              //         Unused
    MaxSignal = 32,
};

typedef struct ::sigaction sigaction_t;

inline int sigaction(Signal signum, const sigaction_t *act, sigaction_t *oldact)
{
    return ::sigaction(static_cast<int>(signum), act, oldact);
}

using sighandler_t = ::sighandler_t;

inline sighandler_t signal(Signal signum, sighandler_t handler)
{
    return ::signal(static_cast<int>(signum), handler);
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
    int add(Signal signum)
    {
        return sigaddset(&_set, static_cast<int>(signum));
    }
    int remove(Signal signum)
    {
        return sigdelset(&_set, static_cast<int>(signum));
    }
    int contains(Signal signum)
    {
        return sigismember(&_set, static_cast<int>(signum));
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

inline int SetSignalMask(SignalHow how, const sigset_t *set, sigset_t *oldset)
{
    return ::pthread_sigmask(how, set, oldset);
}

} // namespace Signal
} // namespace OSAL