/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCF1_SYSTEM_H
#define SSCF1_SYSTEM_H

// For C++/CLI this class gives LNK4210: .CRTMP section exists; there may be unhandled static initializers or terminators
#ifndef _M_CEE

#ifdef _MSC_VER
# pragma warning (disable : 4265) // Suppress warning from Microsoft headers
#endif
#include <functional>
#ifdef _MSC_VER
# pragma warning (default : 4265)
#endif

#if defined(_MSC_VER)
# define noexcept throw()
#endif

namespace Sscf1 {

class System;

/*! Returns a reference to the instance of the System class.
 */
System & system();

/*! The System class provides a platform abstraction for the tracing features.
	This class is a singleton, obtain a reference to the instance with the
	Sscf1::system() function.
 */
class System
{
friend System & system();
public:
	static const char version[];	//!< Version number denoted as [Major].[Minor].[Patch].[Revision]
	static const int revision;		//!< Revision number

	~System(void);

    /*! Adds a trace message handler that receives all messages traced with
        system().traceMsg().
        \param handler      A function that is to called for each message, must
							be thread-safe.
     */
	void registerTraceMsgHandler(const std::function<void(const char *) noexcept> &handler);

    /*! Writes a trace message to the registered trace message handlers. The
        arguments follow the same semantics as printf().
     */
    void traceMsg(const char *, ...) const noexcept;

    /*! Writes a trace message to the registered trace message handlers and
		aborts the application by throwing an exception with the same message.
		The arguments follow the same semantics as printf().
     */
#if defined(_MSC_VER) && (_MSC_VER < 1900)
    void __declspec(noreturn) fail(const char *, ...) const;
#else
	[[noreturn]] void fail(const char *, ...) const;
#endif

	/*! Disables all thread safety checks.
	 */
	void disableThreadSafetyChecks() noexcept;

	/*! Returns true if thread safety checks need to be performed.
	 */
	bool threadSafetyChecksEnabled() const noexcept;

private:
	System(void);
	System(const System &);
	System & operator=(const System &);

private:
    struct Private;
    Private *p;
};

}

#if defined(_MSC_VER)
# undef noexcept
#endif

#endif
#endif
