#pragma once

#define WARNING_PUSH
#define WARNING_PUSH_N(level)
#define WARNING_DISABLE(num)
#define WARNING_POP

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <rpc.h>
#undef min
#undef max

#define SL32(x) x
#define UL32(x) x##U
#define SL64(x) x##LL
#define UL64(x) x##ULL

namespace OSAL {
namespace Network {
    int inet_pton(int af, const char *src, void *dst);
}
}