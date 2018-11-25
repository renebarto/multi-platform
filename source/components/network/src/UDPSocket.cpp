#include "network/UDPSocket.h"

using namespace std;
using namespace Network;

void UDPSocket::Open()
{
    socket::Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
}
