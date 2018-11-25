#include "Network/TCPSocket.h"
//#include <cstring>
//#include <iomanip>
//#include <poll.h>
//#include <sstream>
//#include "Core/DefaultLogger.h"
//#include "Core/Util.h"

using namespace std;
using namespace Network;

void TCPSocket::Open()
{
    socket::Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Stream);
}

bool TCPSocket::Connect(const OSAL::Network::IPV4Address & ipAddress, uint16_t port, OSAL::Network::SocketTimeout timeout)
{
    return socket::Connect(OSAL::Network::Create(ipAddress, port), timeout);
}

bool TCPSocket::Connect(const OSAL::Network::IPV6Address & ipAddress, uint16_t port, OSAL::Network::SocketTimeout timeout)
{
    return socket::Connect(OSAL::Network::Create(ipAddress, port), timeout);
}

bool TCPSocket::Accept(TCPSocket & connectionSocket, OSAL::Network::IPV4Address & clientAddress, uint16_t & port, OSAL::Network::SocketTimeout timeout)
{
    OSAL::Network::EndPointPtr clientEndPoint;
    bool result = socket::Accept(connectionSocket, clientEndPoint, timeout);
    if (result)
    {
        std::shared_ptr<OSAL::Network::IPV4EndPoint> ipV4EndPoint = dynamic_pointer_cast<OSAL::Network::IPV4EndPoint>(clientEndPoint);
        clientAddress = ipV4EndPoint->GetIPAddress();
        port = ipV4EndPoint->GetPort();
    }
    return result;
}

bool TCPSocket::Accept(TCPSocket & connectionSocket, OSAL::Network::IPV6Address & clientAddress, uint16_t & port, OSAL::Network::SocketTimeout timeout)
{
    OSAL::Network::EndPointPtr clientEndPoint;
    bool result = socket::Accept(connectionSocket, clientEndPoint, timeout);
    if (result)
    {
        std::shared_ptr<OSAL::Network::IPV6EndPoint> ipV6EndPoint = dynamic_pointer_cast<OSAL::Network::IPV6EndPoint>(clientEndPoint);
        clientAddress = ipV6EndPoint->GetIPAddress();
        port = ipV6EndPoint->GetPort();
    }
    return result;
}
