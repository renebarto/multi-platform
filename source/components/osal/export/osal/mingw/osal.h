#pragma once

#define WARNING_PUSH
#define WARNING_PUSH_N(level)
#define WARNING_DISABLE(num)
#define WARNING_POP

#define SL32(x) x
#define UL32(x) x##U
#define SL64(x) x##LL
#define UL64(x) x##ULL

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
