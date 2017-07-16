#pragma once

#define WARNING_PUSH __pragma(warning(push))
#define WARNING_PUSH_N(level) __pragma(warning(push, level))
#define WARNING_DISABLE(num) __pragma(warning(disable: num))
#define WARNING_POP __pragma(warning(pop))

#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4574)
#pragma warning(disable: 4668)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#undef min
#undef max
#pragma warning(pop)

#define SL32(x) x
#define UL32(x) x##u
#define SL64(x) x##i64
#define UL64(x) x##ui64
