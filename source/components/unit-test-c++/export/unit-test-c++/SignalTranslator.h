#pragma once

#include <setjmp.h>
#include <signal.h>
#include <osal/platform-defines.h>

namespace UnitTestCpp
{

class SignalTranslator
{
public:
    SignalTranslator();
    ~SignalTranslator();

#if defined(LINUX) || defined(DARWIN)
	static sigjmp_buf * s_jumpTarget;
#endif

private:
#if defined(LINUX) || defined(DARWIN)
	sigjmp_buf m_currentJumpTarget;
    sigjmp_buf * m_oldJumpTarget;

    struct sigaction m_old_SIGFPE_action;
    struct sigaction m_old_SIGTRAP_action;
    struct sigaction m_old_SIGSEGV_action;
    struct sigaction m_old_SIGBUS_action;
    struct sigaction m_old_SIGABRT_action;
    struct sigaction m_old_SIGALRM_action;
#endif
};

#if defined(LINUX) || defined(DARWIN)
    #define UNITTEST_EXTENSION __extension__
#else
    #define UNITTEST_EXTENSION
#endif

#if defined(LINUX) || defined(DARWIN)
#define UNITTEST_THROW_SIGNALS \
    ::UnitTestCpp::SignalTranslator sig; \
    if (UNITTEST_EXTENSION sigsetjmp(*::UnitTestCpp::SignalTranslator::s_jumpTarget, 1) != 0) \
        throw("Unhandled system exception");
#else
#define UNITTEST_THROW_SIGNALS
#endif

} // namespace UnitTestCpp

