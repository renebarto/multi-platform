/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#ifndef SSCFTS1_INTERFACE_BEGIN_H
# define SSCFTS1_INTERFACE_BEGIN_H
#else
# error Missing #include <sscfts1/interface.end.h>
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

#ifndef SSCFTS1_BEGIN_INTERFACE
# error Please define one of SSCFTS1_DECLARE_INTERFACE, SSCFTS1_DEFINE_INTERFACE, SSCFTS1_DECLARE_PROXY, SSCFTS1_DEFINE_PROXY, SSCFTS1_DECLARE_NATIVE_STRUCTS, SSCFTS1_DECLARE_DISPATCHER, or SSCFTS1_DEFINE_DISPATCHER before including interface.begin.h.
#endif

#else // C++/CLR

#include "interface.begin.managedinterface.h"

#endif
