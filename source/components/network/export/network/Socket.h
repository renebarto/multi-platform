#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/net_tstamp.h>
#include <memory>
#include <sstream>
#include <vector>
#include "network/exports.h"
#include "osal/OSAL.h"
WARNING_PUSH
WARNING_DISABLE(4265)
#include <mutex>
WARNING_POP
#include "osal/ByteArray.h"
#include "osal/Network.h"
#include "osal/NetworkEndPoint.h"

namespace Network
{

class NETWORK_EXPORT Socket
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

    void            Open(OSAL::Network::SocketFamily socketFamily, OSAL::Network::SocketType socketType);
    void            Close();
    bool            IsClosed();

    void            SetSocketOption(OSAL::Network::SocketOptionLevel level, OSAL::Network::SocketOption socketOption, void * optionValue, unsigned int optionLength);
    void            GetSocketOption(OSAL::Network::SocketOptionLevel level, OSAL::Network::SocketOption socketOption, void * optionValue, unsigned int & optionLength);
    void            SetSocketOption(OSAL::Network::SocketOption socketOption, void * optionValue, unsigned int optionLength);
    void            GetSocketOption(OSAL::Network::SocketOption socketOption, void * optionValue, unsigned int & optionLength);

    bool            GetSocketOptionBool(OSAL::Network::SocketOption socketOption);
    void            SetSocketOptionBool(OSAL::Network::SocketOption socketOption, bool value);
    int             GetSocketOptionInt(OSAL::Network::SocketOption socketOption);
    void            SetSocketOptionInt(OSAL::Network::SocketOption, int value);

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

    void            Bind(OSAL::Network::EndPointPtr address);

    bool            Connect(OSAL::Network::EndPointPtr serverAddress, OSAL::Network::SocketTimeout timeout);
    void            Listen(int numListeners);
    bool            Accept(Socket & connectionSocket, OSAL::Network::EndPointPtr & clientAddress, OSAL::Network::SocketTimeout timeout);

    void            GetLocalAddress(OSAL::Network::EndPointPtr & address);
    void            GetRemoteAddress(OSAL::Network::EndPointPtr & address);

    size_t          Receive(OSAL::ByteArray & data, int flags);
    size_t          Receive(uint8_t * data, size_t bufferSize, int flags);
    bool            Send(OSAL::ByteArray & data, size_t bytesToSend, int flags);
    bool            Send(const uint8_t * data, size_t bytesToSend, int flags);
    void            SendTo(const OSAL::Network::EndPointPtr & address, const uint8_t * data, size_t bytesToSend);
    size_t          ReceiveFrom(OSAL::Network::EndPointPtr & address, uint8_t * data, size_t bufferSize);

    std::ostream & PrintTo(std::ostream & stream) const;

protected:
    OSAL::Network::SocketFamily _socketFamily;
    OSAL::Network::SocketHandle _socketHandle;

    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;
    Mutex _mutex;
};

inline void PrintTo(Socket const & socket, std::basic_ostream<OSAL::Char> & stream)
{
    socket.PrintTo(stream);
}

} // namespace Network
