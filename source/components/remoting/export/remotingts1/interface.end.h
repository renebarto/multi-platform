/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#undef REMOTINGTS1_BEGIN_INTERFACE
#undef REMOTINGTS1_BEGIN_INTERFACE_VERSION
#undef REMOTINGTS1_BEGIN_METHODS
#undef REMOTINGTS1_INTERFACE_METHOD_0
#undef REMOTINGTS1_INTERFACE_METHOD_1
#undef REMOTINGTS1_INTERFACE_METHOD_2
#undef REMOTINGTS1_INTERFACE_METHOD_3
#undef REMOTINGTS1_INTERFACE_METHOD_4
#undef REMOTINGTS1_INTERFACE_METHOD_5
#undef REMOTINGTS1_INTERFACE_METHOD_ASYNC_0
#undef REMOTINGTS1_INTERFACE_METHOD_ASYNC_1
#undef REMOTINGTS1_INTERFACE_METHOD_ASYNC_2
#undef REMOTINGTS1_INTERFACE_METHOD_ASYNC_3
#undef REMOTINGTS1_INTERFACE_METHOD_ASYNC_4
#undef REMOTINGTS1_INTERFACE_METHOD_ASYNC_5
#undef REMOTINGTS1_END_METHODS
#undef REMOTINGTS1_BEGIN_EVENTS
#undef REMOTINGTS1_INTERFACE_EVENT_0
#undef REMOTINGTS1_INTERFACE_EVENT_1
#undef REMOTINGTS1_INTERFACE_EVENT_2
#undef REMOTINGTS1_INTERFACE_EVENT_3
#undef REMOTINGTS1_INTERFACE_EVENT_4
#undef REMOTINGTS1_INTERFACE_EVENT_5
#undef REMOTINGTS1_END_EVENTS
#undef REMOTINGTS1_END_INTERFACE
#undef REMOTINGTS1_INTERFACE_NAME

#undef REMOTINGTS1_BEGIN_STRUCT
#undef REMOTINGTS1_BEGIN_EXTEND_STRUCT
#undef REMOTINGTS1_STRUCT_MEMBER
#undef REMOTINGTS1_STRUCT_MEMBER_ARRAY
#undef REMOTINGTS1_END_STRUCT

#undef in
#undef inout
#undef reftype

#ifdef _M_CEE
# undef Remoting1
# undef RemotingTs1
# undef std
# undef string
# undef wstring
# undef list
# undef set
# undef vector
#endif

#if defined(_MSC_VER)
# if !defined(_BullseyeCoverage)	// Workaround for bug in Bullseye Coverage
__if_exists(string)		{ $fail$ }	// Prevent Native/.NET interop issues; remove "using namespace std" from header files.
__if_exists(wstring)	{ $fail$ }	// Prevent Native/.NET interop issues; remove "using namespace std" from header files.
__if_exists(list)		{ $fail$ }	// Prevent Native/.NET interop issues; remove "using namespace std" from header files.
__if_exists(set)		{ $fail$ }	// Prevent Native/.NET interop issues; remove "using namespace std" from header files.
__if_exists(vector)		{ $fail$ }	// Prevent Native/.NET interop issues; remove "using namespace std" from header files.
# endif
# pragma warning (default : 4100) // Unreferenced formal parameter
# pragma warning (default : 4101) // Unreferenced local variable
#endif

#undef REMOTINGTS1_INTERFACE_BEGIN_H
