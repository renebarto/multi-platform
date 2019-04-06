/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifdef _MSC_VER
# ifndef _CRT_SECURE_NO_WARNINGS
#  define _CRT_SECURE_NO_WARNINGS 1
# endif
# ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#  define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
# endif
# ifndef _WIN32_WINNT_WINTHRESHOLD
#  define _WIN32_WINNT_WINTHRESHOLD           0x0601
# endif
# ifndef NTDDI_WIN7SP1
#  define NTDDI_WIN7SP1                       0x0601010
# endif
# pragma warning (disable : 4464) // Suppress warning including relative headers
#endif

#include "sscf1/system.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <functional>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif
#if !defined(NDEBUG) && defined(_WIN32)
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif

#if defined(_MSC_VER)
# define noexcept throw()
#endif

// define ANSI/ISO names
#ifdef _MSC_VER
# define snprintf _snprintf
#endif

namespace Sscf1 {

struct System::Private
{
	Private() : threadSafetyChecksEnabled(true) { }

    std::function<void(const char *) noexcept> traceMsgHandler;
	bool threadSafetyChecksEnabled;
};

System::System(void) 
    : p(new Private())
{
#if !defined(NDEBUG) && defined(_WIN32)
	p->traceMsgHandler = [](const char *msg)
	{ 
        char buffer[4096]; buffer[4095] = '\0';
        snprintf(buffer, sizeof(buffer) - 1, "Sscf1: %s\n", msg);

        ::OutputDebugStringA(buffer);
	};
#endif
}

System::~System(void)
{
    delete p;
    p = nullptr;
}

void System::registerTraceMsgHandler(const std::function<void(const char *)> &handler)
{
	// This check is needed as System is a singleton and can be destroyed
	// before other singletons, which may still be using it.
	if (p)
	{
		p->traceMsgHandler = handler;

        traceMsg("SSCF Build %s @ %s", __DATE__, __TIME__);
	}
}


void System::traceMsg(const char *format, ...) const noexcept
{
	char buffer[4096]; buffer[4095] = '\0';

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer) - 1, format, args);
	va_end(args);

	// This check is needed as System is a singleton and can be destroyed
	// before other singletons, which may still be using it.
	if (p && p->traceMsgHandler)
		p->traceMsgHandler(buffer);
}

void System::fail(const char *format, ...) const
{
	char buffer[4096]; buffer[4095] = '\0';
	strcpy(buffer, "FATAL: ");

	va_list args;
	va_start(args, format);
	vsnprintf(buffer + 7, sizeof(buffer) - 8, format, args);
	va_end(args);

	// This check is needed as System is a singleton and can be destroyed
	// before other singletons, which may still be using it.
	if (p && p->traceMsgHandler)
		p->traceMsgHandler(buffer);

	throw std::runtime_error(std::string(buffer));
}

void System::disableThreadSafetyChecks() noexcept
{
	// This check is needed as System is a singleton and can be destroyed
	// before other singletons, which may still be using it.
	if (p)
		p->threadSafetyChecksEnabled = false;
}

bool System::threadSafetyChecksEnabled() const noexcept
{
	// This check is needed as System is a singleton and can be destroyed
	// before other singletons, which may still be using it.
	if (p)
		return p->threadSafetyChecksEnabled;

	return false;
}

System &system()
{
	static System s;
	return s; 
}

}
