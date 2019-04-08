/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef REMOTINGTS1_INTERFACE_BEGIN_H
# define REMOTINGTS1_INTERFACE_BEGIN_H
#else
# error Missing #include <remotingts1/interface.end.h>
#endif

#ifndef _M_CEE

#if defined(_MSC_VER)
# pragma warning (disable : 4100) // Unreferenced formal parameter
# pragma warning (disable : 4101) // Unreferenced local variable
#elif defined(__GNUC__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)))
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "interface.begin.interface.h"
#include "interface.begin.proxy.h"
#include "interface.begin.dispatcher.h"

#ifndef REMOTINGTS1_BEGIN_INTERFACE
# error Please define one of REMOTINGTS1_DECLARE_INTERFACE, REMOTINGTS1_DEFINE_INTERFACE, REMOTINGTS1_DECLARE_PROXY, REMOTINGTS1_DEFINE_PROXY, REMOTINGTS1_DECLARE_NATIVE_STRUCTS, REMOTINGTS1_DECLARE_DISPATCHER, or REMOTINGTS1_DEFINE_DISPATCHER before including interface.begin.h.
#endif

#else // C++/CLR

#include "interface.begin.managedinterface.h"

#endif
