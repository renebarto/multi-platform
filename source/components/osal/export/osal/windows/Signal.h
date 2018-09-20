#pragma once

#include <signal.h>
#include "osal/OSAL.h"
#include "osal/Unused.h"
WARNING_PUSH
WARNING_DISABLE(4265)
#include <bitset>
WARNING_POP

namespace OSAL {
namespace Signals {

enum class Signal
{
    SigInvalid = -1,
    SigHangup = SigInvalid,             // Term    Hangup detected on controlling terminal or death of controlling process
    SigInterrupt = SIGINT,              // Term    Interrupt from keyboard (ANSI)
    SigQuit = SigInvalid,               // Core    Quit from keyboard (POSIX)
    SigIllegalInstruction = SIGILL,     // Core    Illegal Instruction (ANSI)
    SigTrap = SigInvalid,               // Core    Trace Trap (POSIX)
    SigAbort = SIGABRT,                 // Core    Abort signal from abort(3) (ANSI) / IOT trap (4.2 BSD)
    SigBus = SigInvalid,                // Core    BUS error (4.2 BSD)
    SigFloatingPoint = SIGFPE,          // Core    Floating-point exception (ANSI)
    SigKill = SigInvalid,               // Term    Kill signal (POSIX)
    SigUser1 = SigInvalid,              // Core    User-defined signal 1 (POSIX)
    SigSegmentViolation = SIGSEGV,      // Core    Invalid memory reference / Segment violation (POSIX)
    SigUser2 = SigInvalid,              // Core    User-defined signal 2 (POSIX)
    SigPipe = SigInvalid,               // Term    Broken pipe: write to pipe with no readers; see pipe(7) (POSIX)
    SigAlarm = SigInvalid,              // Term    Timer signal from alarm(2) (POSIX)
    SigTerminate = SIGTERM,             // Term    Termination signal (ANSI)
    SigStackFault = SigInvalid,         // Core    Stack fault
    SigChild = SigInvalid,              // Ign     Child stopped or terminated (POSIX)
    SigContinue = SigInvalid,           // Cont    Continue if stopped (POSIX)
    SigStop = SigInvalid,               // Stop    Stop process, unblockable (POSIX)
    SigTerminalStop = SigInvalid,       // Stop    Stop typed at terminal (POSIX)
    SigTTYIn = SigInvalid,              // Stop    Terminal input for background process (POSIX)
    SigTTYOut = SigInvalid,             // Stop    Terminal output for background process (POSIX)
    SigUrgent = SigInvalid,             //         Urgent condition on socket (4.2 BSD)
    SigCPULimit = SigInvalid,           //         CPU limit exceeded (4.2 BSD)
    SigFileSizeLimit = SigInvalid,      //         File size limit exceeded (4.2 BSD)
    SigVirtualAlarm = SigInvalid,       //         Virtual alarm clock (4.2 BSD)
    SigProfileAlarm = SigInvalid,       //         Profiling alarm clock (4.2 BSD)
    SigWindowSize = SigInvalid,         //         Window size change (4.3 BSD, Sun)
    SigPoll = SigInvalid,               //         Pollable event occurred (System V)
    SigIO = SigInvalid,                 //         I/O now possible (4.2 BSD)
    SigPower = SigInvalid,              //         Power failure restart (System V)
    SigSystemCall = SigInvalid,         //         Bad system call
    SigUnused = SigInvalid,             //         Unused
    MaxSignal = 32,
};

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

inline int sigaction(int UNUSED(signum), const sigaction_t * UNUSED(act), sigaction_t * UNUSED(oldact))
{
    return -1;
}

using sighandler_t = ::_crt_signal_t;

inline sighandler_t signal(int signum, sighandler_t handler)
{
    return ::signal(signum, handler);
}

enum SignalHow
{
    Block = 0,
    Unblock = 1,
    SetMask = 2,
};

OSAL_EXPORT int SetSignalMask(SignalHow how, const sigset_t *set, sigset_t *oldset);

class SignalSet
{
public:
    SignalSet()
        : _set()
    {
        Clear();
    }
    int Clear()
    {
        _set.set.reset();
        return 0;
    }
    bool IsEmpty()
    {
        return (_set.set.count() == 0);
    }
    int Fill()
    {
        _set.set.set();
        return 0;
    }
    int Add(Signal signum)
    {
        if ((static_cast<int>(signum) < 0) || (static_cast<size_t>(signum) >= _set.set.size()))
            return -1;
        _set.set.set(static_cast<size_t>(signum));
        return 0;
    }
    int Remove(Signal signum)
    {
        if ((static_cast<int>(signum) < 0) || (static_cast<size_t>(signum) >= _set.set.size()))
            return -1;
        _set.set.reset(static_cast<size_t>(signum));
        return 0;
    }
    int Contains(Signal signum)
    {
        if ((static_cast<int>(signum) < 0) || (static_cast<size_t>(signum) >= _set.set.size()))
            return -1;
        return (_set.set.test(static_cast<size_t>(signum))) ? 1 : 0;
    }
    int And(const SignalSet & lhs, const SignalSet & rhs)
    {
        this->_set.set = lhs._set.set & rhs._set.set;
        return 0;
    }
    int Or(const SignalSet & lhs, const SignalSet & rhs)
    {
        this->_set.set = lhs._set.set | rhs._set.set;
        return 0;
    }
    const sigset_t & GetSet() const
    {
        return _set;
    }
    int SetMask(SignalHow operation) const
    {
        return SetSignalMask(operation, &_set, nullptr);
    }
    int Block() const
    {
        return SetSignalMask(SignalHow::Block, &_set, nullptr);
    }
    int Unblock() const
    {
        return SetSignalMask(SignalHow::Unblock, &_set, nullptr);
    }
    int SetMask() const
    {
        return SetSignalMask(SignalHow::SetMask, &_set, nullptr);
    }

private:
    sigset_t _set;
};

} // namespace Signals
} // namespace OSAL