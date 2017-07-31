#pragma once

#include <memory>
WARNING_PUSH
WARNING_DISABLE(4265)
#include <mutex>
WARNING_POP
#include <sstream>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/net_tstamp.h>

#include "core/ByteArray.h"
#include "osal/Network.h"
#include "network/Address.h"

namespace Network
{

enum class SocketType
{
    Stream = SOCK_STREAM,
    Datagram = SOCK_DGRAM,
    Raw = SOCK_RAW,
    SeqPacket = SOCK_SEQPACKET,
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

class Socket
{
protected:
    static const uint32_t TimeWait;

public:
    Socket();
    Socket(const Socket & other);
    Socket(Socket && other);
    virtual ~Socket();

    Socket & operator = (Socket const & other);
    Socket & operator = (Socket && other);

    OSAL::Network::SocketHandle GetHandle() const;
    void            SetHandle(OSAL::Network::SocketHandle handle);

    void            Open(SocketFamily socketFamily, SocketType socketType);
    void            Close();
    bool            IsClosed();

    void            SetSocketOption(SocketOptionLevel level, SocketOption socketOption, void * optionValue, int optionLength);
    void            GetSocketOption(SocketOptionLevel level, SocketOption socketOption, void * optionValue, unsigned int * optionLength);
    void            SetSocketOption(SocketOption socketOption, void * optionValue, unsigned int optionLength);
    void            GetSocketOption(SocketOption socketOption, void * optionValue, unsigned int * optionLength);

    bool            GetSocketOptionBool(SocketOption socketOption);
    void            SetSocketOptionBool(SocketOption socketOption, bool value);
    int             GetSocketOptionInt(SocketOption socketOption);
    void            SetSocketOptionInt(SocketOption socketOption, int value);

    bool            GetBroadcastOption();
    void            SetBroadcastOption(bool value);
    bool            GetBlockingMode();
    void            SetBlockingMode(bool value);
    bool            GetReuseAddress();
    void            SetReuseAddress(bool value);
    int             GetReceiveTimeout();
    void            SetReceiveTimeout(int timeoutMS);
    int             GetSendTimeout();
    void            SetSendTimeout(int timeoutMS);

    void            Bind(sockaddr const * address, socklen_t addressLength);

    bool            Connect(sockaddr const * serverAddress, socklen_t serverAddressLength, OSAL::Network::SocketTimeout timeout);
    void            Listen(int numListeners);
    bool            Accept(Socket & connectionSocket, sockaddr * clientAddress, socklen_t * clientAddressLength, OSAL::Network::SocketTimeout timeout);

    void            GetLocalAddress(sockaddr * address, socklen_t * addressLength);
    void            GetRemoteAddress(sockaddr * address, socklen_t * addressLength);

    Core::ByteArray Receive(int flags);
    size_t          Receive(uint8_t * data, size_t bufferSize, int flags);
    bool            Send(Core::ByteArray & data, size_t bytesToSend, int flags);
    bool            Send(const uint8_t * data, size_t bytesToSend, int flags);
    void            SendTo(sockaddr * address, socklen_t addressLength, const uint8_t * data, size_t bytesToSend);
    size_t          ReceiveFrom(sockaddr * address, socklen_t * addressLength, uint8_t * data, size_t bufferSize);

    virtual std::string ToString() const;

protected:
    OSAL::Network::SocketHandle socketHandle;

    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;
    Mutex mutex;
};

} // namespace Network
