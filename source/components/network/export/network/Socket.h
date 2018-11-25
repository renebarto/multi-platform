#pragma once

#include <memory>
#include <sstream>
#include <vector>
#include "network/exports.h"
#include "osal/osal.h"
WARNING_PUSH
WARNING_DISABLE(4265)
#include <mutex>
WARNING_POP
#include "osal/bytearray.h"
#include "osal/Network.h"
#include "osal/NetworkEndPoint.h"

namespace Network
{

class NETWORK_EXPORT socket
{
protected:
    static const OSAL::Network::SocketTimeout TimeWait;

public:
    socket();
    socket(const socket & other);
    socket(socket && other);
    virtual ~socket();

    socket & operator = (socket const & other);
    socket & operator = (socket && other);

    OSAL::Network::SocketHandle GetHandle() const;
    void            SetHandle(OSAL::Network::SocketHandle handle);

    void            Open(OSAL::Network::SocketFamily socketFamily, OSAL::Network::SocketType socketType);
    void            Close();
    bool            IsClosed();

    void            SetSocketOption(OSAL::Network::SocketOptionLevel level, OSAL::Network::SocketOption socketOption, void * optionValue, socklen_t optionLength);
    void            GetSocketOption(OSAL::Network::SocketOptionLevel level, OSAL::Network::SocketOption socketOption, void * optionValue, socklen_t & optionLength);
    void            SetSocketOption(OSAL::Network::SocketOption socketOption, void * optionValue, socklen_t optionLength);
    void            GetSocketOption(OSAL::Network::SocketOption socketOption, void * optionValue, socklen_t & optionLength);

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
    bool            Accept(socket & connectionSocket, OSAL::Network::EndPointPtr & clientAddress, OSAL::Network::SocketTimeout timeout);

    void            GetLocalAddress(OSAL::Network::EndPointPtr & address);
    void            GetRemoteAddress(OSAL::Network::EndPointPtr & address);

    size_t          Receive(OSAL::bytearray & data, int flags);
    size_t          Receive(uint8_t * data, size_t bufferSize, int flags);
    bool            Send(OSAL::bytearray & data, size_t bytesToSend, int flags);
    bool            Send(const uint8_t * data, size_t bytesToSend, int flags);
    void            SendTo(const OSAL::Network::EndPointPtr & address, const uint8_t * data, size_t bytesToSend);
    size_t          ReceiveFrom(OSAL::Network::EndPointPtr & address, uint8_t * data, size_t bufferSize);

    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const;

protected:
    OSAL::Network::SocketFamily _socketFamily;
    OSAL::Network::SocketHandle _socketHandle;
    bool _isBlocking;
};

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & socket::PrintTo(std::basic_ostream<Elem, Traits> & s) const
{
    s << OSAL::System::TypeName(*this) << " handle = " << static_cast<long>(GetHandle());
    return s;
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & PrintTo(const socket & value, std::basic_ostream<Elem, Traits> & s)
{
    return value.PrintTo(s);
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & operator<<(std::basic_ostream<Elem, Traits> &s, const socket & value)
{
    return value.PrintTo(s);
}

} // namespace Network
