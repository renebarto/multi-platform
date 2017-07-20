#include "network/AdapterList.h"

#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <sys/un.h>
#include <core/Core.h>
#include <core/DefaultLogger.h>
#include <network/Address.h>
#include <network/IPV4Address.h>
#include <network/IPV6Address.h>
#include <network/DomainSocketAddress.h>
#include <network/Adapter.h>
#include <network/MACAddress.h>

using namespace std;
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
    Core::ThrowOnError(__func__, __FILE__, __LINE__, getifaddrs(&adapters));

    for (ifaddrs * currentAdapter = adapters; currentAdapter != nullptr; currentAdapter = currentAdapter->ifa_next)
    {
        AddressPtr localAddress;
        AddressPtr netmaskaddress;
        AddressPtr broadcastAddress;
        AddressPtr destAddress;
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
                        Core::TheLogger().Error("Network", gai_strerror(result));
                        return false;
                    }
                    localAddress = make_shared<IPV4Address>(reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_addr)->sin_addr.s_addr);
                    netmaskaddress = make_shared<IPV4Address>(reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_netmask)->sin_addr.s_addr);
                    if ((currentAdapter->ifa_flags & IFF_BROADCAST) != 0)
                        broadcastAddress = make_shared<IPV4Address>(reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_ifu.ifu_broadaddr)->sin_addr.s_addr);
                    if ((currentAdapter->ifa_flags & IFF_POINTOPOINT) != 0)
                        destAddress = make_shared<IPV4Address>(reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_ifu.ifu_dstaddr)->sin_addr.s_addr);
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
                        Core::TheLogger().Error("Network", gai_strerror(result));
                        return false;
                    }
                    localAddress = make_shared<IPV6Address>(reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_addr)->sin6_addr.__in6_u.__u6_addr8);
                    netmaskaddress = make_shared<IPV6Address>(reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_netmask)->sin6_addr.__in6_u.__u6_addr8);
                    if (((currentAdapter->ifa_flags & IFF_BROADCAST) != 0) && (currentAdapter->ifa_ifu.ifu_broadaddr != nullptr))
                        broadcastAddress = make_shared<IPV6Address>(reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_ifu.ifu_broadaddr)->sin6_addr.__in6_u.__u6_addr8);
                    if (((currentAdapter->ifa_flags & IFF_POINTOPOINT) != 0) && (currentAdapter->ifa_ifu.ifu_dstaddr != nullptr))
                        destAddress = make_shared<IPV6Address>(reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_ifu.ifu_dstaddr)->sin6_addr.__in6_u.__u6_addr8);
                }
                break;
            case AF_UNIX:
                localAddress = make_shared<DomainSocketAddress>(reinterpret_cast<sockaddr_un *>(currentAdapter->ifa_addr)->sun_path);
                break;
            case AF_PACKET:
                sockaddr_ll * address = reinterpret_cast<sockaddr_ll *>(currentAdapter->ifa_addr);
                localAddress = make_shared<MACAddress>(Core::ByteArray(address->sll_addr, address->sll_halen));
                break;
        }
        _adapters.push_back(make_shared<Adapter>(currentAdapter->ifa_name, localAddress, netmaskaddress, broadcastAddress, destAddress, static_cast<AdapterFlags>(currentAdapter->ifa_flags)));
    }

    freeifaddrs(adapters);
    return true;
}
