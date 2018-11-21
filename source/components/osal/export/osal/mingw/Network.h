#pragma once

namespace OSAL {
namespace Network {

typedef int SocketTimeout;
typedef int SocketHandle;

OSAL_EXPORT extern const SocketHandle InvalidHandleValue;
OSAL_EXPORT extern const SocketTimeout InfiniteTimeout;

} // namespace Network
} // namespace OSAL