#pragma once

#include "osal/NetworkAddress.h"
#include <osal/IPV4Address.h>
#include <osal/IPV6Address.h>
#include <osal/linux/DomainSocketAddress.h>

namespace OSAL {
namespace Network {

typedef int SocketTimeout;
typedef int SocketHandle;

OSAL_EXPORT extern const SocketHandle InvalidHandleValue;
OSAL_EXPORT extern const SocketTimeout InfiniteTimeout;

inline OSAL_EXPORT int Bind(SocketHandle socketHandle, AddressPtr address)
{
    return ::bind(socketHandle,
                  reinterpret_cast<const sockaddr *>(address->GetBytes().Data()),
                  static_cast<socklen_t>(address->GetBytes().Size()));
}

inline OSAL_EXPORT int Connect(SocketHandle socketHandle, AddressPtr serverAddress)
{
    return ::connect(socketHandle,
                     reinterpret_cast<const sockaddr *>(serverAddress->GetBytes().Data()),
                     static_cast<socklen_t>(serverAddress->GetBytes().Size()));
}

inline OSAL_EXPORT SocketHandle Accept(SocketHandle socketHandle, SocketFamily family, AddressPtr & clientAddress)
{
    switch (family)
    {
        case SocketFamily ::InternetV4:
            clientAddress = std::make_shared<IPV4Address>();
            break;
        case SocketFamily ::InternetV6:
            clientAddress = std::make_shared<IPV6Address>();
            break;
        case SocketFamily ::Unix:
            clientAddress = std::make_shared<DomainSocketAddress>();
            break;
        default:
            return InvalidHandleValue;
    }

    socklen_t size = static_cast<socklen_t>(clientAddress->GetBytes().Size());
    SocketHandle result = ::accept(socketHandle, reinterpret_cast<sockaddr *>(clientAddress->GetBytes().Data()), &size);
    assert(size == clientAddress->GetBytes().Size());
    return result;
}

inline OSAL_EXPORT int GetSockName(SocketHandle socketHandle, SocketFamily family, AddressPtr & address)
{
    switch (family)
    {
        case SocketFamily ::InternetV4:
            address = std::make_shared<IPV4Address>();
            break;
        case SocketFamily ::InternetV6:
            address = std::make_shared<IPV6Address>();
            break;
        case SocketFamily ::Unix:
            address = std::make_shared<DomainSocketAddress>();
            break;
        default:
            return EINVAL;
    }

    socklen_t size = static_cast<socklen_t>(address->GetBytes().Size());
    int result = ::getsockname(socketHandle, reinterpret_cast<sockaddr *>(address->GetBytes().Data()), &size);
    assert(size == address->GetBytes().Size());
    return result;
}

inline OSAL_EXPORT int GetPeerName(SocketHandle socketHandle, SocketFamily family, AddressPtr & address)
{
    switch (family)
    {
        case SocketFamily ::InternetV4:
            address = std::make_shared<IPV4Address>();
            break;
        case SocketFamily ::InternetV6:
            address = std::make_shared<IPV6Address>();
            break;
        case SocketFamily ::Unix:
            address = std::make_shared<DomainSocketAddress>();
            break;
        default:
            return EINVAL;
    }

    socklen_t size = static_cast<socklen_t>(address->GetBytes().Size());
    int result = ::getpeername(socketHandle, reinterpret_cast<sockaddr *>(address->GetBytes().Data()), &size);
    assert(size == address->GetBytes().Size());
    return result;
}

} // namespace Network
} // namespace OSAL