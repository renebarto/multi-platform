#pragma once

#include <net/if.h>
#include <memory>
#include "network/Address.h"

namespace Network
{

enum class AdapterFlags : unsigned int
{
    InterfaceUp =           IFF_UP,             // Interface is running.
    ValidBroadcast =        IFF_BROADCAST,      // Valid broadcast address set.
    Debug =                 IFF_DEBUG,          // Internal debugging flag.
    Loopback =              IFF_LOOPBACK,       // Interface is a loopback interface.
    PointToPoint =          IFF_POINTOPOINT,    // Interface is a point-to-point link.
    Running =               IFF_RUNNING,        // Resources allocated.
    NoARP =                 IFF_NOARP,          // No arp protocol, L2 destination address not set.
    Promiscuous =           IFF_PROMISC,        // Interface is in promiscuous mode.
    NoTrailers =            IFF_NOTRAILERS,     // Avoid use of trailers.
    AllMulticast =          IFF_ALLMULTI,       // Receive all multicast packets.
    LoadBalancingMaster =   IFF_MASTER,         // Master of a load balancing bundle.
    LoadBalancingSlave =    IFF_SLAVE,          // Slave of a load balancing bundle.
    SupportMulticast =      IFF_MULTICAST,      // Supports multicast
    PortSelect =            IFF_PORTSEL,        // Is able to select media type via ifmap.
    AutoMedia =             IFF_AUTOMEDIA,      // Auto media selection active.
    Dynamic =               IFF_DYNAMIC,        // The addresses are lost when the interface goes down.
};

class Adapter
{
public:
    Adapter(const std::string & name, AddressPtr localAddress, AddressPtr netmask,
            AddressPtr broadcastAddress, AddressPtr destAddress, AdapterFlags flags);

    const std::string & Name() { return _name; }
    const AddressPtr LocalAddress() { return _localAddress; }
    const AddressPtr NetMask() { return _netmask; }
    const AddressPtr BroadcastAddress() { return _broadcastAddress; }
    const AddressPtr DestAddress() { return _destAddress; }
    SocketFamily Family() { return LocalAddress()->Family(); }
    AdapterFlags flags() { return _flags; }

private:
    std::string _name;
    AddressPtr _localAddress;
    AddressPtr _netmask;
    AddressPtr _broadcastAddress;
    AddressPtr _destAddress;
    AdapterFlags _flags;
};

} // namespace Network
