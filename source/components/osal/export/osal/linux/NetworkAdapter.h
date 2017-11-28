#pragma once

#include <net/if.h>
#include <memory>
#include "osal/exports.h"
#include "osal/NetworkEndPoint.h"

namespace OSAL {
namespace Network {

enum class AdapterFlags : unsigned int
{
    InterfaceUp = IFF_UP,             // Interface is running.
    ValidBroadcast = IFF_BROADCAST,      // Valid broadcast address set.
    Debug = IFF_DEBUG,          // Internal debugging flag.
    Loopback = IFF_LOOPBACK,       // Interface is a loopback interface.
    PointToPoint = IFF_POINTOPOINT,    // Interface is a point-to-point link.
    Running = IFF_RUNNING,        // Resources allocated.
    NoARP = IFF_NOARP,          // No arp protocol, L2 destination address not set.
    Promiscuous = IFF_PROMISC,        // Interface is in promiscuous mode.
    NoTrailers = IFF_NOTRAILERS,     // Avoid use of trailers.
    AllMulticast = IFF_ALLMULTI,       // Receive all multicast packets.
    LoadBalancingMaster = IFF_MASTER,         // Master of a load balancing bundle.
    LoadBalancingSlave = IFF_SLAVE,          // Slave of a load balancing bundle.
    SupportMulticast = IFF_MULTICAST,      // Supports multicast
    PortSelect = IFF_PORTSEL,        // Is able to select media type via ifmap.
    AutoMedia = IFF_AUTOMEDIA,      // Auto media selection active.
    Dynamic = IFF_DYNAMIC,        // The addresses are lost when the interface goes down.
};

class OSAL_EXPORT Adapter
{
public:
    Adapter(const String & name, EndPointPtr localAddress, EndPointPtr netmask,
            EndPointPtr broadcastAddress, EndPointPtr destAddress, AdapterFlags flags);

    const String & Name() const
    { return _name; }

    const EndPointPtr LocalAddress() const
    { return _localAddress; }

    const EndPointPtr NetMask()
    { return _netmask; }

    const EndPointPtr BroadcastAddress() const
    { return _broadcastAddress; }

    const EndPointPtr DestAddress() const
    { return _destAddress; }

    SocketFamily Family() const
    { return LocalAddress()->Family(); }

    AdapterFlags Flags() const
    { return _flags; }

private:
    String _name;
    EndPointPtr _localAddress;
    EndPointPtr _netmask;
    EndPointPtr _broadcastAddress;
    EndPointPtr _destAddress;
    AdapterFlags _flags;
};

} // namespace Network
} // namespace OSAL