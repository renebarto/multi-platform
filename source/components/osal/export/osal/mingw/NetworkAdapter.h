#pragma once

#include <memory>
#include "osal/exports.h"
#include "osal/osal.h"
#include "osal/FlagOperators.h"
#include "osal/NetworkEndPoint.h"

namespace OSAL {
namespace Network {

enum class AdapterFlags : unsigned int {
    InterfaceUp = IFF_UP,               // Interface is running.
    ValidBroadcast = IFF_BROADCAST,     // Valid broadcast address set.
    Debug = 0,                          // Internal debugging flag.
    Loopback = IFF_LOOPBACK,            // Interface is a loopback interface.
    PointToPoint = IFF_POINTTOPOINT,    // Interface is a point-to-point link.
    Running = 0,                        // Resources allocated.
    NoARP = 0,                          // No arp protocol, L2 destination address not set.
    Promiscuous = 0,                    // Interface is in promiscuous mode.
    NoTrailers = 0,                     // Avoid use of trailers.
    AllMulticast = 0,                   // Receive all multicast packets.
    LoadBalancingMaster = 0,            // Master of a load balancing bundle.
    LoadBalancingSlave = 0,             // Slave of a load balancing bundle.
    SupportMulticast = IFF_MULTICAST,   // Supports multicast
    PortSelect = 0,                     // Is able to select media type via ifmap.
    AutoMedia = 0,                      // Auto media selection active.
    Dynamic = 0,                        // The addresses are lost when the interface goes down.
};

} // namespace Network

template<>
struct is_flag<OSAL::Network::AdapterFlags>
{
    static constexpr bool value = true;
};

namespace Network {

class OSAL_EXPORT Adapter
{
public:
    Adapter(const std::string & name, EndPointPtr localAddress, EndPointPtr netmask,
                   EndPointPtr broadcastAddress, EndPointPtr destAddress, AdapterFlags flags)
        : _name(name)
        , _localAddress(localAddress)
        , _netmask(netmask)
        , _broadcastAddress(broadcastAddress)
        , _destAddress(destAddress)
        , _flags(flags)
    {
    }

    const std::string & Name() { return _name; }
    const EndPointPtr LocalAddress() { return _localAddress; }
    const EndPointPtr NetMask() { return _netmask; }
    const EndPointPtr BroadcastAddress() { return _broadcastAddress; }
    const EndPointPtr DestAddress() { return _destAddress; }
    SocketFamily Family() { return LocalAddress()->Family(); }
    AdapterFlags Flags() { return _flags; }

private:
    std::string _name;
    EndPointPtr _localAddress;
    EndPointPtr _netmask;
    EndPointPtr _broadcastAddress;
    EndPointPtr _destAddress;
    AdapterFlags _flags;
};

} // namespace Network
} // namespace OSAL