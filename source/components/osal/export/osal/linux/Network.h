#pragma once

#include "osal/linux/Adapter.h"
#include "osal/linux/AdapterList.h"

namespace OSAL {
namespace Network {

typedef int SocketTimeout;
typedef int SocketHandle;

OSAL_EXPORT extern const SocketHandle InvalidHandleValue;
OSAL_EXPORT extern const SocketTimeout InfiniteTimeout;

} // namespace Network
} // namespace OSAL