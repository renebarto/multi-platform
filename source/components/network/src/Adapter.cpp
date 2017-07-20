#include "network/Adapter.h"

using namespace Network;

Adapter::Adapter(const std::string & name, AddressPtr localAddress, AddressPtr netmask,
                 AddressPtr broadcastAddress, AddressPtr destAddress, AdapterFlags flags)
    : _name(name)
    , _localAddress(localAddress)
    , _netmask(netmask)
    , _broadcastAddress(broadcastAddress)
    , _destAddress(destAddress)
    , _flags(flags)
{

}