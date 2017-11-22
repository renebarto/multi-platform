#include "osal/NetworkAdapter.h"

using namespace OSAL;
using namespace Network;

Adapter::Adapter(const std::string & name, EndPointPtr localAddress, EndPointPtr netmask,
                 EndPointPtr broadcastAddress, EndPointPtr destAddress, AdapterFlags flags)
    : _name(name)
    , _localAddress(localAddress)
    , _netmask(netmask)
    , _broadcastAddress(broadcastAddress)
    , _destAddress(destAddress)
    , _flags(flags)
{

}