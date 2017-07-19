#include "network/AdapterList.h"

#include <ifaddrs.h>
#include <core/Core.h>

using namespace Network;

AdapterList::AdapterList()
    : _adapters()
{

}

void AdapterList::ReScan()
{
    ifaddrs * adapters;
    Core::ThrowOnError(__func__, __FILE__, __LINE__, getifaddrs(&adapters));

    for (ifaddrs * currentAdapter = adapters; currentAdapter != nullptr; currentAdapter = currentAdapter->ifa_next)
    {

    }

    freeifaddrs(adapters);
}
