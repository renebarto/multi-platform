#pragma once

#include "network/Socket.h"

namespace Network
{

class TCPSocket : public socket
{
public:
    TCPSocket() {}
    virtual ~TCPSocket() {}

    void Open();
    bool Connect(const OSAL::Network::IPV4Address & ipAddress, uint16_t port, OSAL::Network::SocketTimeout timeout);
    bool Connect(const OSAL::Network::IPV6Address & ipAddress, uint16_t port, OSAL::Network::SocketTimeout timeout);
    bool Accept(TCPSocket & connectionSocket, OSAL::Network::IPV4Address & clientAddress, uint16_t & port, OSAL::Network::SocketTimeout timeout);
    bool Accept(TCPSocket & connectionSocket, OSAL::Network::IPV6Address & clientAddress, uint16_t & port, OSAL::Network::SocketTimeout timeout);
//
//protected:
//    using Socket::Open;
};

} // namespace Network
