#include "osal/NetworkAdapterList.h"

#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <sys/un.h>
#include <osal/NetworkAdapter.h>
#include <osal/Exception.h>
#include <osal/IPV4EndPoint.h>
#include <osal/IPV6EndPoint.h>
#include <osal/linux/DomainSocketAddress.h>
#include <osal/MACAddress.h>

using namespace std;
using namespace OSAL;
using namespace Network;

AdapterList::AdapterList()
    : _adapters()
{
    ReScan();
}

bool AdapterList::ReScan()
{
    _adapters.clear();
    ifaddrs * adapters;
    OSAL::ThrowOnError(__func__, __FILE__, __LINE__, getifaddrs(&adapters));

    for (ifaddrs * currentAdapter = adapters; currentAdapter != nullptr; currentAdapter = currentAdapter->ifa_next)
    {
        EndPointPtr localAddress;
        EndPointPtr netmaskaddress;
        EndPointPtr broadcastAddress;
        EndPointPtr destAddress;
        char host[NI_MAXHOST];
        switch (currentAdapter->ifa_addr->sa_family)
        {
            case AF_INET:
                {
                    int result = getnameinfo(currentAdapter->ifa_addr,
                                             sizeof(struct sockaddr_in),
                                             host, NI_MAXHOST,
                                             nullptr, 0, NI_NUMERICHOST);
                    if (result != 0)
                    {
                        return false;
                    }
                    sockaddr_in * address = reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_addr);
                    localAddress = make_shared<IPV4EndPoint>(IPV4Address(address->sin_addr.s_addr), address->sin_port);
                    address = reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_netmask);
                    netmaskaddress = make_shared<IPV4EndPoint>(IPV4Address(address->sin_addr.s_addr), address->sin_port);
                    if ((currentAdapter->ifa_flags & IFF_BROADCAST) != 0)
                    {
                        address = reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_ifu.ifu_broadaddr);
                        broadcastAddress = make_shared<IPV4EndPoint>(IPV4Address(address->sin_addr.s_addr), address->sin_port);
                    }
                    if ((currentAdapter->ifa_flags & IFF_POINTOPOINT) != 0)
                    {
                        address = reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_ifu.ifu_dstaddr);
                        destAddress = make_shared<IPV4EndPoint>(IPV4Address(address->sin_addr.s_addr), address->sin_port);
                    }
                }
                break;
            case AF_INET6:
                {
                    int result = getnameinfo(currentAdapter->ifa_addr,
                                             sizeof(struct sockaddr_in6),
                                             host, NI_MAXHOST,
                                             nullptr, 0, NI_NUMERICHOST);
                    if (result != 0)
                    {
                        return false;
                    }
                    sockaddr_in6 * address = reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_addr);
                    localAddress = make_shared<IPV6EndPoint>(address->sin6_addr.__in6_u.__u6_addr8, address->sin6_port,
                                                             address->sin6_flowinfo, address->sin6_scope_id);
                    address = reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_netmask);
                    netmaskaddress = make_shared<IPV6EndPoint>(address->sin6_addr.__in6_u.__u6_addr8, address->sin6_port,
                                                               address->sin6_flowinfo, address->sin6_scope_id);
                    if (((currentAdapter->ifa_flags & IFF_BROADCAST) != 0) && (currentAdapter->ifa_ifu.ifu_broadaddr != nullptr))
                    {
                        address = reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_ifu.ifu_broadaddr);
                        broadcastAddress = make_shared<IPV6EndPoint>(address->sin6_addr.__in6_u.__u6_addr8, address->sin6_port,
                                                                     address->sin6_flowinfo, address->sin6_scope_id);
                    }
                    if (((currentAdapter->ifa_flags & IFF_POINTOPOINT) != 0) && (currentAdapter->ifa_ifu.ifu_dstaddr != nullptr))
                    {
                        address = reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_ifu.ifu_dstaddr);
                        destAddress = make_shared<IPV6EndPoint>(address->sin6_addr.__in6_u.__u6_addr8, address->sin6_port,
                                                                address->sin6_flowinfo, address->sin6_scope_id);
                    }
                }
                break;
            case AF_UNIX:
                localAddress = make_shared<DomainSocketAddress>(reinterpret_cast<sockaddr_un *>(currentAdapter->ifa_addr)->sun_path);
                break;
            case AF_PACKET:
                sockaddr_ll * address = reinterpret_cast<sockaddr_ll *>(currentAdapter->ifa_addr);
                localAddress = make_shared<MACAddress>(OSAL::bytearray(address->sll_addr, address->sll_halen));
                break;
        }
        _adapters.push_back(make_shared<Adapter>(currentAdapter->ifa_name, localAddress, netmaskaddress, broadcastAddress, destAddress, static_cast<AdapterFlags>(currentAdapter->ifa_flags)));
    }

    freeifaddrs(adapters);
    return true;
}
