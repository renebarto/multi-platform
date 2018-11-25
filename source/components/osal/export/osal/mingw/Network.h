#pragma once

#include <thread>
#include <osal/NetworkEndPoint.h>
#include <osal/IPV4EndPoint.h>
#include <osal/IPV6EndPoint.h>
#include <osal/Unused.h>

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

inline int GetSocketError()
{
    return WSAGetLastError();
}

inline std::string GetSocketErrorMessage(int errorCode)
{
    char buffer[1024];
    ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorCode, 0, buffer, sizeof(buffer), nullptr);
    return std::string(buffer);
}

inline int InitializeSocketLibrary()
{
    WSAData wsaData;
    return WSAStartup(2, &wsaData);
}

inline int UninitializeSocketLibrary()
{
    return (WSACleanup() == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline int CreateSocket(SocketFamily socketFamily, SocketType socketType, SocketHandle & handle)
{
    handle = InvalidHandleValue;
    SocketHandle result = ::socket(static_cast<int>(socketFamily), static_cast<int>(socketType), 0);
    if (result == static_cast<SocketHandle>(SOCKET_ERROR))
    {
        return GetSocketError();
    }
    handle = result;
    return 0;
}

inline int CloseSocket(SocketHandle socketHandle)
{
    return (::closesocket(socketHandle) == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline int SetSocketOption(SocketHandle socketHandle,
                           SocketOptionLevel level, SocketOption socketOption,
                           void * optionValue, socklen_t optionLength)
{
    return (::setsockopt(socketHandle,
                         static_cast<int>(level), static_cast<int>(socketOption),
                         reinterpret_cast<char *>(optionValue), optionLength) == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline int GetSocketOption(SocketHandle socketHandle,
                           SocketOptionLevel level, SocketOption socketOption,
                           void * optionValue, socklen_t & optionLength)
{
    return (::getsockopt(socketHandle,
                         static_cast<int>(level), static_cast<int>(socketOption),
                         reinterpret_cast<char *>(optionValue), &optionLength) == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline int FileControl(SocketHandle socketHandle, int cmd)
{
    return (::ioctlsocket(socketHandle, cmd, nullptr) == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline int FileControl(SocketHandle socketHandle, int cmd, int flag)
{
    return (::ioctlsocket(socketHandle, cmd, reinterpret_cast<u_long *>(&flag)) == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline int GetBlockingMode(SocketHandle UNUSED(socketHandle), bool & blockingMode)
{
    // Can't determine this in Windows
    blockingMode = false;
    return EINVAL;
}

inline int SetBlockingMode(SocketHandle socketHandle, bool value)
{
    int mode = (value ? 0 : 1);
    return OSAL::Network::FileControl(socketHandle, FIONBIO, mode);
}

inline int Bind(SocketHandle socketHandle, EndPointPtr address)
{
    return (::bind(socketHandle,
                   reinterpret_cast<const sockaddr *>(address->GetBytes().data()),
                   static_cast<socklen_t>(address->GetBytes().size())) == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline int Listen(SocketHandle socketHandle, int numListeners)
{
    return (::listen(socketHandle, numListeners) == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline int Connect(SocketHandle socketHandle, const EndPointPtr & serverAddress, bool & connected,
                   SocketTimeout timeout = InfiniteTimeout)
{
    connected = false;
    SetBlockingMode(socketHandle, timeout == InfiniteTimeout);

    int result = ::connect(socketHandle,
                           reinterpret_cast<const sockaddr *>(serverAddress->GetBytes().data()),
                           static_cast<socklen_t>(serverAddress->GetBytes().size()));
    if (result == SOCKET_ERROR)
    {
        int errorCode = GetSocketError();

        if (errorCode == WSAEWOULDBLOCK)
        {
            fd_set socketHandleSet;
            FD_SET(socketHandle, &socketHandleSet);
            timeval timeVal { timeout / 1000, (timeout % 1000) * 1000 };

            int selectResult = select(1, nullptr, &socketHandleSet, nullptr, &timeVal);
            if (selectResult == SOCKET_ERROR)
            {
                errorCode = GetSocketError();
                return errorCode;
            } else
            {
                connected = (selectResult == 0);
                result = 0;
            }
        }
        else
            return errorCode;
    }

    if (result == 0)
        result = SetBlockingMode(socketHandle, true);
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

inline int GetSockName(SocketHandle socketHandle, SocketFamily family, EndPointPtr & address)
{
    address = nullptr;
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
    return (result == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline int GetPeerName(SocketHandle socketHandle, SocketFamily family, EndPointPtr & address)
{
    address = nullptr;
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
    return (result == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline ssize_t Receive(SocketHandle socketHandle, uint8_t * data, size_t bufferSize, int flags)
{
    return (::recv(socketHandle, reinterpret_cast<char *>(data), static_cast<int>(bufferSize), flags) == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline ssize_t Send(SocketHandle socketHandle, const uint8_t * data, size_t bufferSize, int flags)
{
    return (::send(socketHandle, reinterpret_cast<const char *>(data), static_cast<int>(bufferSize), flags) == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline ssize_t ReceiveFrom(SocketHandle socketHandle, uint8_t * data, size_t bufferSize, int flags, SocketFamily family, EndPointPtr & address)
{
    address = nullptr;
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
    ssize_t result = ::recvfrom(socketHandle, reinterpret_cast<char *>(data), static_cast<int>(bufferSize), flags,
                                reinterpret_cast<sockaddr *>(address->GetBytes().data()), &size);
    return (result == SOCKET_ERROR) ? GetSocketError() : 0;
}

inline ssize_t SendTo(SocketHandle socketHandle, const uint8_t * data, size_t bufferSize, int flags, const EndPointPtr & address)
{
    return (::sendto(socketHandle, reinterpret_cast<const char *>(data), static_cast<int>(bufferSize), flags,
                     reinterpret_cast<const sockaddr *>(address->GetBytes().data()),
                     static_cast<socklen_t>(address->GetBytes().size())) == SOCKET_ERROR) ? GetSocketError() : 0;
}

} // namespace Network
} // namespace OSAL