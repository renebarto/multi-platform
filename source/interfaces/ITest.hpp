//-------------------------------------------------------------------
// Copyright(c) 2014 Philips Medical Systems Nederland B.V.
//-------------------------------------------------------------------

// All units are SI base units (m, s, A) or directly derived (V, Gy, °C)

#include <sscfts1/interface.begin.h>
#ifdef SSCFTS1_BEGIN_INTERFACE

SSCFTS1_BEGIN_INTERFACE(ITest)
    SSCFTS1_BEGIN_METHODS(ITest)
        SSCFTS1_INTERFACE_METHOD_0(int, ITest, Start)
        SSCFTS1_INTERFACE_METHOD_0(int, ITest, Stop)
    SSCFTS1_END_METHODS

    SSCFTS1_BEGIN_EVENTS(ITest)
        SSCFTS1_INTERFACE_EVENT_0(ITest, Callback)
    SSCFTS1_END_EVENTS
SSCFTS1_END_INTERFACE

#endif
#include <sscfts1/interface.end.h>
