/*-----------------------------------------------------------------------------
 | Copyright: Copyright(c) 2012, Philips Medical Systems Nederland B.V.       |
 | Author:    A.J. Admiraal, iXR Innovation                                   |
 -----------------------------------------------------------------------------*/

#undef SSCFTS1_BEGIN_INTERFACE
#undef SSCFTS1_BEGIN_INTERFACE_VERSION
#undef SSCFTS1_BEGIN_METHODS
#undef SSCFTS1_INTERFACE_METHOD_0
#undef SSCFTS1_INTERFACE_METHOD_1
#undef SSCFTS1_INTERFACE_METHOD_2
#undef SSCFTS1_INTERFACE_METHOD_3
#undef SSCFTS1_INTERFACE_METHOD_4
#undef SSCFTS1_INTERFACE_METHOD_5
#undef SSCFTS1_INTERFACE_METHOD_ASYNC_0
#undef SSCFTS1_INTERFACE_METHOD_ASYNC_1
#undef SSCFTS1_INTERFACE_METHOD_ASYNC_2
#undef SSCFTS1_INTERFACE_METHOD_ASYNC_3
#undef SSCFTS1_INTERFACE_METHOD_ASYNC_4
#undef SSCFTS1_INTERFACE_METHOD_ASYNC_5
#undef SSCFTS1_END_METHODS
#undef SSCFTS1_BEGIN_EVENTS
#undef SSCFTS1_INTERFACE_EVENT_0
#undef SSCFTS1_INTERFACE_EVENT_1
#undef SSCFTS1_INTERFACE_EVENT_2
#undef SSCFTS1_INTERFACE_EVENT_3
#undef SSCFTS1_INTERFACE_EVENT_4
#undef SSCFTS1_INTERFACE_EVENT_5
#undef SSCFTS1_END_EVENTS
#undef SSCFTS1_END_INTERFACE
#undef SSCFTS1_INTERFACE_NAME

#undef SSCFTS1_BEGIN_STRUCT
#undef SSCFTS1_BEGIN_EXTEND_STRUCT
#undef SSCFTS1_STRUCT_MEMBER
#undef SSCFTS1_STRUCT_MEMBER_ARRAY
#undef SSCFTS1_END_STRUCT

#undef in
#undef inout
#undef reftype

#ifdef _M_CEE
# undef Sscf1
# undef SscfTs1
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

#undef SSCFTS1_INTERFACE_BEGIN_H
