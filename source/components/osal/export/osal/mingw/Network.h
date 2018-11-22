#pragma once

#include "osal/NetworkEndPoint.h"
#include <osal/IPV4EndPoint.h>
#include <osal/IPV6EndPoint.h>

namespace OSAL {
namespace Network {

typedef int SocketTimeout;
typedef SOCKET SocketHandle;

enum class SocketType
{
    Stream = SOCK_STREAM,
    Datagram = SOCK_DGRAM,
    Raw = SOCK_RAW,
    SeqPacket = SOCK_SEQPACKET,
    DatagramReliable = SOCK_RDM,
    Packet = -1,
};

enum class SocketOptionLevel
{
    Socket = SOL_SOCKET,
};

enum class SocketOption
{
    Debug = SO_DEBUG,
    ReuseAddress = SO_REUSEADDR,
    Type = SO_TYPE,
    Error = SO_ERROR,
    DontRoute = SO_DONTROUTE,
    Broadcast = SO_BROADCAST,
    SendBuffer = SO_SNDBUF,
    ReceiveBuffer = SO_RCVBUF,
    SendBufferForce = 0, //SO_SNDBUFFORCE,
    ReceiveBufferForce = 0, //SO_RCVBUFFORCE,
    KeepAlive = SO_KEEPALIVE,
    OutOfBandInline = SO_OOBINLINE,
    NoCheck = 0, //SO_NO_CHECK,
    Priority = 0, //SO_PRIORITY,
    Linger = SO_LINGER,
    BSDCompatibility = 0, //SO_BSDCOMPAT,
    ReceiveTimeout = SO_RCVTIMEO,
    SendTimeout = SO_SNDTIMEO,
    BindToDevice = 0, //SO_BINDTODEVICE,
    Timestamping = 0, //SO_TIMESTAMPING
};

OSAL_EXPORT extern const SocketHandle InvalidHandleValue;
OSAL_EXPORT extern const SocketTimeout InfiniteTimeout;

inline OSAL_EXPORT SocketHandle CreateSocket(SocketFamily socketFamily, SocketType socketType)
{
    return ::socket(static_cast<int>(socketFamily), static_cast<int>(socketType), 0);
}

inline OSAL_EXPORT int CloseSocket(SocketHandle socketHandle)
{
    return ::closesocket(socketHandle);
}

inline OSAL_EXPORT int SetSocketOption(SocketHandle socketHandle,
                                       SocketOptionLevel level, SocketOption socketOption,
                                       void * optionValue, unsigned int optionLength)
{
    return ::setsockopt(socketHandle,
                        static_cast<int>(level), static_cast<int>(socketOption),
                        reinterpret_cast<char *>(optionValue), optionLength);
}

inline OSAL_EXPORT int GetSocketOption(SocketHandle socketHandle,
                                       SocketOptionLevel level, SocketOption socketOption,
                                       void * optionValue, unsigned int & optionLength)
{
    return ::getsockopt(socketHandle,
                        static_cast<int>(level), static_cast<int>(socketOption),
                        reinterpret_cast<char *>(optionValue), reinterpret_cast<socklen_t *>(&optionLength));
}

inline OSAL_EXPORT int FileControl(SocketHandle socketHandle, int cmd)
{
    return ioctlsocket(socketHandle, cmd, nullptr);
}

inline OSAL_EXPORT int FileControl(SocketHandle socketHandle, int cmd, int flag)
{
    return ioctlsocket(socketHandle, cmd, reinterpret_cast<u_long *>(&flag));
}

inline OSAL_EXPORT int Bind(SocketHandle socketHandle, EndPointPtr address)
{
    return ::bind(socketHandle,
                  reinterpret_cast<const sockaddr *>(address->GetBytes().data()),
                  static_cast<socklen_t>(address->GetBytes().size()));
}

inline OSAL_EXPORT int Connect(SocketHandle socketHandle, const EndPointPtr & serverAddress)
{
    return ::connect(socketHandle,
                     reinterpret_cast<const sockaddr *>(serverAddress->GetBytes().data()),
                     static_cast<socklen_t>(serverAddress->GetBytes().size()));
}

inline OSAL_EXPORT SocketHandle Accept(SocketHandle socketHandle, SocketFamily family, EndPointPtr & clientAddress)
{
    switch (family)
    {
        case SocketFamily ::InternetV4:
            clientAddress = std::make_shared<IPV4EndPoint>();
            break;
        case SocketFamily ::InternetV6:
            clientAddress = std::make_shared<IPV6EndPoint>();
            break;
        default:
            return InvalidHandleValue;
    }

    socklen_t size = static_cast<socklen_t>(clientAddress->GetBytes().size());
    SocketHandle result = ::accept(socketHandle, reinterpret_cast<sockaddr *>(clientAddress->GetBytes().data()), &size);
    ASSERT(size == static_cast<socklen_t>(clientAddress->GetBytes().size()));
    return result;
}

inline OSAL_EXPORT int GetSockName(SocketHandle socketHandle, SocketFamily family, EndPointPtr & address)
{
    switch (family)
    {
        case SocketFamily ::InternetV4:
            address = std::make_shared<IPV4EndPoint>();
            break;
        case SocketFamily ::InternetV6:
            address = std::make_shared<IPV6EndPoint>();
            break;
        default:
            return EINVAL;
    }

    socklen_t size = static_cast<socklen_t>(address->GetBytes().size());
    int result = ::getsockname(socketHandle, reinterpret_cast<sockaddr *>(address->GetBytes().data()), &size);
    ASSERT(size == static_cast<socklen_t>(address->GetBytes().size()));
    return result;
}

inline OSAL_EXPORT int GetPeerName(SocketHandle socketHandle, SocketFamily family, EndPointPtr & address)
{
    switch (family)
    {
        case SocketFamily ::InternetV4:
            address = std::make_shared<IPV4EndPoint>();
            break;
        case SocketFamily ::InternetV6:
            address = std::make_shared<IPV6EndPoint>();
            break;
        default:
            return EINVAL;
    }

    socklen_t size = static_cast<socklen_t>(address->GetBytes().size());
    int result = ::getpeername(socketHandle, reinterpret_cast<sockaddr *>(address->GetBytes().data()), &size);
    ASSERT(size == static_cast<socklen_t>(address->GetBytes().size()));
    return result;
}

inline OSAL_EXPORT ssize_t Receive(SocketHandle socketHandle, uint8_t * data, size_t bufferSize, int flags)
{
    return ::recv(socketHandle, reinterpret_cast<char *>(data), bufferSize, flags);
}

inline OSAL_EXPORT ssize_t Send(SocketHandle socketHandle, const uint8_t * data, size_t bufferSize, int flags)
{
    return ::send(socketHandle, reinterpret_cast<const char *>(data), bufferSize, flags);
}

inline OSAL_EXPORT ssize_t ReceiveFrom(SocketHandle socketHandle, uint8_t * data, size_t bufferSize, int flags, SocketFamily family, EndPointPtr &address)
{
    switch (family)
    {
        case SocketFamily::InternetV4:
            address = std::make_shared<IPV4EndPoint>();
            break;
        case SocketFamily::InternetV6:
            address = std::make_shared<IPV6EndPoint>();
            break;
        default:
            return EINVAL;
    }

    socklen_t size = static_cast<socklen_t>(address->GetBytes().size());
    ssize_t result = ::recvfrom(socketHandle, reinterpret_cast<char *>(data), bufferSize, flags,
                                reinterpret_cast<sockaddr *>(address->GetBytes().data()), &size);
    return result;
}

inline OSAL_EXPORT ssize_t SendTo(SocketHandle socketHandle, const uint8_t * data, size_t bufferSize, int flags, const EndPointPtr & address)
{
    return ::sendto(socketHandle, reinterpret_cast<const char *>(data), bufferSize, flags,
                    reinterpret_cast<const sockaddr *>(address->GetBytes().data()),
                    static_cast<socklen_t>(address->GetBytes().size()));
}

} // namespace Network
} // namespace OSAL