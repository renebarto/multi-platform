#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <thread>
#include <osal/NetworkEndPoint.h>
#include <osal/linux/DomainSocketAddress.h>
#include <osal/IPV4EndPoint.h>
#include <osal/IPV6EndPoint.h>

namespace OSAL {
namespace Network {

typedef int SocketTimeout;
typedef int SocketHandle;

enum class SocketType
{
    Stream = SOCK_STREAM,
    Datagram = SOCK_DGRAM,
    Raw = SOCK_RAW,
    SeqPacket = SOCK_SEQPACKET,
    DatagramReliable = SOCK_RDM,
    Packet = SOCK_PACKET,
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
    SendBufferForce = SO_SNDBUFFORCE,
    ReceiveBufferForce = SO_RCVBUFFORCE,
    KeepAlive = SO_KEEPALIVE,
    OutOfBandInline = SO_OOBINLINE,
    NoCheck = SO_NO_CHECK,
    Priority = SO_PRIORITY,
    Linger = SO_LINGER,
    BSDCompatibility = SO_BSDCOMPAT,
    ReceiveTimeout = SO_RCVTIMEO,
    SendTimeout = SO_SNDTIMEO,
    BindToDevice = SO_BINDTODEVICE,
    Timestamping = SO_TIMESTAMPING
};

OSAL_EXPORT extern const SocketHandle InvalidHandleValue;
OSAL_EXPORT extern const SocketTimeout InfiniteTimeout;

inline int GetSocketError()
{
    return errno;
}

inline std::string GetSocketErrorMessage(int errorCode)
{
    return strerror(errorCode);
}

inline int InitializeSocketLibrary()
{
    return 0;
}

inline int UninitializeSocketLibrary()
{
    return 0;
}

inline int CreateSocket(SocketFamily socketFamily, SocketType socketType, SocketHandle & handle)
{
    handle = InvalidHandleValue;
    SocketHandle result = ::socket(static_cast<int>(socketFamily), static_cast<int>(socketType), 0);
    if (result == -1)
    {
        return GetSocketError();
    }
    handle = result;
    return 0;
}

inline int CloseSocket(SocketHandle socketHandle)
{
    return ::close(socketHandle);
}

inline int SetSocketOption(SocketHandle socketHandle,
                                       SocketOptionLevel level, SocketOption socketOption,
                                       void * optionValue, unsigned int optionLength)
{
    return ::setsockopt(socketHandle, static_cast<int>(level), static_cast<int>(socketOption), optionValue, optionLength);
}

inline int GetSocketOption(SocketHandle socketHandle,
                                       SocketOptionLevel level, SocketOption socketOption,
                                       void * optionValue, unsigned int & optionLength)
{
    return ::getsockopt(socketHandle,
                        static_cast<int>(level), static_cast<int>(socketOption),
                        optionValue, reinterpret_cast<socklen_t *>(&optionLength));
}

inline int FileControl(SocketHandle socketHandle, int cmd)
{
    return fcntl(socketHandle, cmd);
}

inline int FileControl(SocketHandle socketHandle, int cmd, int flag)
{
    return fcntl(socketHandle, cmd, flag);
}

inline int GetBlockingMode(SocketHandle socketHandle, bool & blockingMode)
{
    blockingMode = false;
    int flags = FileControl(socketHandle, F_GETFL);
    if (flags == -1)
    {
        return errno;
    }
    blockingMode = (flags & O_NONBLOCK) == 0;
    return 0;
}

inline int SetBlockingMode(SocketHandle socketHandle, bool value)
{
    int flags = OSAL::Network::FileControl(socketHandle, F_GETFL);
    if (flags == -1)
    {
        return errno;

    }
    int errorCode = OSAL::Network::FileControl(socketHandle, F_SETFL,
                                               value ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK));
    if (errorCode == -1)
    {
        return errno;
    }
    return 0;
}

inline int Bind(SocketHandle socketHandle, EndPointPtr address)
{
    return ::bind(socketHandle,
                  reinterpret_cast<const sockaddr *>(address->GetBytes().data()),
                  static_cast<socklen_t>(address->GetBytes().size()));
}

inline int Listen(SocketHandle socketHandle, int numListeners)
{
    return (::listen(socketHandle, numListeners) == -1) ? GetSocketError() : 0;
}

inline int Connect(SocketHandle socketHandle, const EndPointPtr & serverAddress, bool & connected, SocketTimeout timeout = InfiniteTimeout)
{
    connected = false;
    if (timeout != OSAL::Network::InfiniteTimeout)
    {
        SetBlockingMode(socketHandle, false);
    } else
    {
        SetBlockingMode(socketHandle, true);
    }

    int result = ::connect(socketHandle,
                           reinterpret_cast<const sockaddr *>(serverAddress->GetBytes().data()),
                           static_cast<socklen_t>(serverAddress->GetBytes().size()));
    if (result == -1)
    {
        int errorCode = errno;

        if ((errorCode == EINPROGRESS) || (errorCode == EALREADY))
        {
            pollfd descriptor;
            descriptor.fd = socketHandle;
            descriptor.events = POLLIN | POLLOUT;
            descriptor.revents = 0;
            int pollResult = ::poll(&descriptor, 1, timeout);
            if (pollResult == -1)
            {
                return errno;
            } else if (pollResult == 0)
            {
                connected = false;
                result = 0;
            } else
            {
                connected = !(descriptor.revents & POLLHUP);
                result = 0;
            }
        } else
        if ((errorCode != EWOULDBLOCK) && (errorCode != EAGAIN))
            return errorCode;
    }

    if (result == 0)
        SetBlockingMode(socketHandle, true);
    return result;
}

inline int Accept(SocketHandle socketHandle, SocketFamily family, EndPointPtr & clientAddress, SocketHandle & acceptHandle,
                  SocketTimeout timeout = InfiniteTimeout)
{
    acceptHandle = InvalidHandleValue;
    const SocketTimeout TimeWait = 10;

    SetBlockingMode(socketHandle, timeout == InfiniteTimeout);

    OSAL::Network::SocketTimeout waitTime = 0;
    if (timeout != OSAL::Network::InfiniteTimeout)
    {
        waitTime = timeout;
    }

    OSAL::Network::SocketHandle result = 0;
    do
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
                return EINVAL;
        }
        socklen_t size = static_cast<socklen_t>(clientAddress->GetBytes().size());
        result = ::accept(socketHandle, reinterpret_cast<sockaddr *>(clientAddress->GetBytes().data()), &size);
        ASSERT(size == static_cast<socklen_t>(clientAddress->GetBytes().size()));

        if (acceptHandle == OSAL::Network::InvalidHandleValue)
        {
            int errorCode = errno;

            if (((errorCode == EWOULDBLOCK) || (errorCode == EAGAIN)) && (waitTime > 0))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(TimeWait));
                waitTime -= std::min(waitTime, TimeWait);
            } else
            {
                return errorCode;
            }
        }
    } while ((result == OSAL::Network::InvalidHandleValue) && (waitTime > 0));

    int errorCode = SetBlockingMode(socketHandle, true);
    if (errorCode != 0)
        return errorCode;
    acceptHandle = result;
    return 0;
}

inline SocketHandle Accept(SocketHandle socketHandle, SocketFamily family, EndPointPtr & clientAddress)
{
    switch (family)
    {
        case SocketFamily ::InternetV4:
            clientAddress = std::make_shared<IPV4EndPoint>();
            break;
        case SocketFamily ::InternetV6:
            clientAddress = std::make_shared<IPV6EndPoint>();
            break;
        case SocketFamily ::Unix:
            clientAddress = std::make_shared<DomainSocketAddress>();
            break;
        default:
            return InvalidHandleValue;
    }

    socklen_t size = static_cast<socklen_t>(clientAddress->GetBytes().size());
    SocketHandle result = ::accept(socketHandle, reinterpret_cast<sockaddr *>(clientAddress->GetBytes().data()), &size);
    ASSERT(size == clientAddress->GetBytes().size());
    return result;
}

inline int GetSockName(SocketHandle socketHandle, SocketFamily family, EndPointPtr & address)
{
    switch (family)
    {
        case SocketFamily ::InternetV4:
            address = std::make_shared<IPV4EndPoint>();
            break;
        case SocketFamily ::InternetV6:
            address = std::make_shared<IPV6EndPoint>();
            break;
        case SocketFamily ::Unix:
            address = std::make_shared<DomainSocketAddress>();
            break;
        default:
            return EINVAL;
    }

    socklen_t size = static_cast<socklen_t>(address->GetBytes().size());
    int result = ::getsockname(socketHandle, reinterpret_cast<sockaddr *>(address->GetBytes().data()), &size);
    ASSERT(size == address->GetBytes().size());
    return result;
}

inline int GetPeerName(SocketHandle socketHandle, SocketFamily family, EndPointPtr & address)
{
    switch (family)
    {
        case SocketFamily ::InternetV4:
            address = std::make_shared<IPV4EndPoint>();
            break;
        case SocketFamily ::InternetV6:
            address = std::make_shared<IPV6EndPoint>();
            break;
        case SocketFamily ::Unix:
            address = std::make_shared<DomainSocketAddress>();
            break;
        default:
            return EINVAL;
    }

    socklen_t size = static_cast<socklen_t>(address->GetBytes().size());
    int result = ::getpeername(socketHandle, reinterpret_cast<sockaddr *>(address->GetBytes().data()), &size);
    ASSERT(size == address->GetBytes().size());
    return result;
}

inline ssize_t Receive(SocketHandle socketHandle, uint8_t * data, size_t bufferSize, int flags)
{
    return ::recv(socketHandle, data, bufferSize, flags);
}

inline ssize_t Send(SocketHandle socketHandle, const uint8_t * data, size_t bufferSize, int flags)
{
    return ::send(socketHandle, data, bufferSize, flags);
}

inline ssize_t ReceiveFrom(SocketHandle socketHandle, uint8_t * data, size_t bufferSize, int flags, SocketFamily family, EndPointPtr &address)
{
    switch (family)
    {
        case SocketFamily::InternetV4:
            address = std::make_shared<IPV4EndPoint>();
            break;
        case SocketFamily::InternetV6:
            address = std::make_shared<IPV6EndPoint>();
            break;
        case SocketFamily::Unix:
            address = std::make_shared<DomainSocketAddress>();
            break;
        default:
            return EINVAL;
    }

    socklen_t size = static_cast<socklen_t>(address->GetBytes().size());
    ssize_t result = ::recvfrom(socketHandle, data, bufferSize, flags,
                            reinterpret_cast<sockaddr *>(address->GetBytes().data()), &size);
    return result;
}

inline ssize_t SendTo(SocketHandle socketHandle, const uint8_t * data, size_t bufferSize, int flags, const EndPointPtr & address)
{
    return ::sendto(socketHandle, data, bufferSize, flags,
                    reinterpret_cast<const sockaddr *>(address->GetBytes().data()),
                    static_cast<socklen_t>(address->GetBytes().size()));
}

} // namespace Network
} // namespace OSAL