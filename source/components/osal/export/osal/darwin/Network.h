#pragma once

namespace OSAL {
namespace Network {

typedef int SocketTimeout;
typedef int SocketHandle;

OSAL_EXPORT const SocketHandle InvalidHandleValue;
OSAL_EXPORT const SocketTimeout InfiniteTimeout;

} // namespace Network
} // namespace OSAL