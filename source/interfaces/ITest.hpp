//-------------------------------------------------------------------
// Copyright(c) 2014 Philips Medical Systems Nederland B.V.
//-------------------------------------------------------------------

// All units are SI base units (m, s, A) or directly derived (V, Gy, °C)

#include <remotingts1/interface.begin.h>
#ifdef REMOTINGTS1_BEGIN_INTERFACE

REMOTINGTS1_BEGIN_INTERFACE(ITest)
    REMOTINGTS1_BEGIN_METHODS(ITest)
        REMOTINGTS1_INTERFACE_METHOD_0(int, ITest, Start)
        REMOTINGTS1_INTERFACE_METHOD_0(int, ITest, Stop)
    REMOTINGTS1_END_METHODS

    REMOTINGTS1_BEGIN_EVENTS(ITest)
        REMOTINGTS1_INTERFACE_EVENT_0(ITest, Callback)
    REMOTINGTS1_END_EVENTS
REMOTINGTS1_END_INTERFACE

#endif
#include <remotingts1/interface.end.h>
