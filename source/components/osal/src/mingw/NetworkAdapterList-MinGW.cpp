#include "osal/NetworkAdapterList.h"

#include <osal/osal.h>
#include <osal/NetworkAdapter.h>
#include <osal/NetworkEndPoint.h>
#include <osal/IPV4EndPoint.h>
#include <inaddr.h>

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

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    // default to unspecified address family (both)
    ULONG family = AF_UNSPEC;

    ULONG addressesLength = 0;

    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = nullptr;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = nullptr;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = nullptr;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = nullptr;
    IP_ADAPTER_PREFIX *pPrefix = nullptr;

    addressesLength = sizeof(IP_ADAPTER_ADDRESSES);
    IP_ADAPTER_ADDRESSES * addresses = reinterpret_cast<IP_ADAPTER_ADDRESSES *>(malloc(addressesLength));

    // Make an initial call to GetAdaptersAddresses to get the
    // size needed into the addressesLength variable
    if (GetAdaptersAddresses(family, flags, nullptr, addresses, &addressesLength) == ERROR_BUFFER_OVERFLOW)
    {
        free(addresses);
        addresses = reinterpret_cast<IP_ADAPTER_ADDRESSES *>(malloc(addressesLength));
    }

    if (addresses == nullptr)
    {
        return false;
    }
    // Make a second call to GetAdapters Addresses to get the
    // actual data we want
    cout << "Memory allocated for GetAdapterAddresses = " << addressesLength << " bytes" << endl;
    cout << "Calling GetAdaptersAddresses function with family = ";
    if (family == AF_INET)
        cout << "AF_INET";
    if (family == AF_INET6)
        cout << "AF_INET6";
    if (family == AF_UNSPEC)
        cout << "AF_UNSPEC";
    cout << endl;

    DWORD result = GetAdaptersAddresses(family, flags, nullptr, addresses, &addressesLength);

    if (result == NO_ERROR)
    {
        // If successful, output some information from the data we received
        PIP_ADAPTER_ADDRESSES pCurrAddresses = addresses;
        while (pCurrAddresses)
        {
            std::string name = OSAL::Strings::WStringToString(pCurrAddresses->FriendlyName);
            AdapterFlags adapterFlags {};
            if (pCurrAddresses->OperStatus == IfOperStatusUp)
                adapterFlags |= IFF_UP;
            if (pCurrAddresses->Flags)
            {

            }
            EndPointPtr localAddress;
            EndPointPtr netmaskaddress;
            EndPointPtr broadcastAddress;
            EndPointPtr destAddress;

            cout << "Length of the IP_ADAPTER_ADDRESS struct: " << pCurrAddresses->Length << endl;
            cout << "IfIndex (IPv4 interface): " << pCurrAddresses->IfIndex << endl;
            cout << "Adapter name: " << pCurrAddresses->AdapterName << endl;

            pUnicast = pCurrAddresses->FirstUnicastAddress;
            if (pUnicast != nullptr)
            {
                int i;
                for (i = 0; pUnicast != nullptr; i++)
                {
                    SocketFamily addressFamily = static_cast<SocketFamily>(pUnicast->Address.lpSockaddr->sa_family);
                    cout << addressFamily << endl;
                    if (addressFamily == SocketFamily::InternetV4)
                    {
                        EndPointPtr address = std::make_shared<IPV4EndPoint>(reinterpret_cast<sockaddr_in *>(pUnicast->Address.lpSockaddr)->sin_addr.S_un.S_addr, reinterpret_cast<sockaddr_in *>(pUnicast->Address.lpSockaddr)->sin_port);
                        cout << *address << endl;
                    }
                    pUnicast = pUnicast->Next;
                }
                cout << "Number of Unicast Addresses: " <<  i << endl;
            } else
                cout << "No Unicast Addresses" << endl;

            pAnycast = pCurrAddresses->FirstAnycastAddress;
            if (pAnycast)
            {
                int i;
                for (i = 0; pAnycast != nullptr; i++)
                    pAnycast = pAnycast->Next;
                cout << "Number of Anycast Addresses: " << i << endl;
            } else
                cout << "No Anycast Addresses" << endl;

            pMulticast = pCurrAddresses->FirstMulticastAddress;
            if (pMulticast)
            {
                int i;
                for (i = 0; pMulticast != nullptr; i++)
                    pMulticast = pMulticast->Next;
                cout << "Number of Multicast Addresses: %d" << i << endl;
            } else
                cout << "No Multicast Addresses" << endl;

            pDnServer = pCurrAddresses->FirstDnsServerAddress;
            if (pDnServer)
            {
                int i;
                for (i = 0; pDnServer != nullptr; i++)
                    pDnServer = pDnServer->Next;
                cout << "Number of DNS Server Addresses: " << i << endl;
            } else
                cout << "No DNS Server Addresses" << endl;

            cout << "DNS Suffix: " << OSAL::Strings::WStringToString(pCurrAddresses->DnsSuffix).c_str() << endl;
            cout << "Description: " << OSAL::Strings::WStringToString(pCurrAddresses->Description).c_str() << endl;
            cout << "Friendly name: " << OSAL::Strings::WStringToString(pCurrAddresses->FriendlyName).c_str() << endl;

            if (pCurrAddresses->PhysicalAddressLength != 0)
            {
                cout << "Physical address: ";
                for (unsigned i = 0; i < pCurrAddresses->PhysicalAddressLength; i++)
                {
                    if (i == (pCurrAddresses->PhysicalAddressLength - 1))
                        cout << hex << setw(2) << setfill('0') << (int) pCurrAddresses->PhysicalAddress[i] << endl << dec;
                    else
                        cout << hex << setw(2) << setfill('0') << (int) pCurrAddresses->PhysicalAddress[i] << "-";
                }
            }
            cout << "Flags: " << pCurrAddresses->Flags << endl;
            cout << "Mtu: " << pCurrAddresses->Mtu << endl;
            cout << "IfType: " << pCurrAddresses->IfType << endl;
            cout << "OperStatus: " << (int)pCurrAddresses->OperStatus << endl;
            cout << "Ipv6IfIndex (IPv6 interface): " << pCurrAddresses->Ipv6IfIndex << endl;
            cout << "ZoneIndices (hex): ";
            for (int i = 0; i < 16; i++)
                printf("%lx ", pCurrAddresses->ZoneIndices[i]);
            cout << endl;

            pPrefix = pCurrAddresses->FirstPrefix;
            if (pPrefix)
            {
                int i;
                for (i = 0; pPrefix != nullptr; i++)
                    pPrefix = pPrefix->Next;
                cout << "Number of IP Adapter Prefix entries: " << i << endl;
            } else
                cout << "No IP Adapter Prefix entries" << endl;

            cout << endl;

            _adapters.push_back(make_shared<Adapter>(name, localAddress, netmaskaddress, broadcastAddress, destAddress, static_cast<AdapterFlags>(flags)));

            pCurrAddresses = pCurrAddresses->Next;
        }
    } else
    {
        cout << "Call to GetAdaptersAddresses failed with error: " << result << endl;
        if (result == ERROR_NO_DATA)
            cout << "No addresses were found for the requested parameters" << endl;
        else 
        {
            char * lpMsgBuf = nullptr;
            if (FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   // Default language
                               (LPSTR)&lpMsgBuf, 0, nullptr))
            {
                cout << "Error: " << lpMsgBuf;
                LocalFree(lpMsgBuf);
                free(addresses);
                return false;
            }
        }
    }
    free(addresses);

//    ifaddrs * adapters;
//    OSAL::ThrowOnError(__func__, __FILE__, __LINE__, getifaddrs(&adapters));

//    for (ifaddrs * currentAdapter = adapters; currentAdapter != nullptr; currentAdapter = currentAdapter->ifa_next)
//    {
//        EndPointPtr localAddress;
//        EndPointPtr netmaskaddress;
//        EndPointPtr broadcastAddress;
//        EndPointPtr destAddress;
//        char host[NI_MAXHOST];
//        switch (currentAdapter->ifa_addr->sa_family)
//        {
//            case AF_INET:
//                {
//                    int result = getnameinfo(currentAdapter->ifa_addr,
//                                             sizeof(struct sockaddr_in),
//                                             host, NI_MAXHOST,
//                                             nullptr, 0, NI_NUMERICHOST);
//                    if (result != 0)
//                    {
//                        return false;
//                    }
//                    sockaddr_in * address = reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_addr);
//                    localAddress = make_shared<IPV4EndPoint>(IPV4Address(address->sin_addr.s_addr), address->sin_port);
//                    address = reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_netmask);
//                    netmaskaddress = make_shared<IPV4EndPoint>(IPV4Address(address->sin_addr.s_addr), address->sin_port);
//                    if ((currentAdapter->ifa_flags & IFF_BROADCAST) != 0)
//                    {
//                        address = reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_ifu.ifu_broadaddr);
//                        broadcastAddress = make_shared<IPV4EndPoint>(IPV4Address(address->sin_addr.s_addr), address->sin_port);
//                    }
//                    if ((currentAdapter->ifa_flags & IFF_POINTOPOINT) != 0)
//                    {
//                        address = reinterpret_cast<sockaddr_in *>(currentAdapter->ifa_ifu.ifu_dstaddr);
//                        destAddress = make_shared<IPV4EndPoint>(IPV4Address(address->sin_addr.s_addr), address->sin_port);
//                    }
//                }
//                break;
//            case AF_INET6:
//                {
//                    int result = getnameinfo(currentAdapter->ifa_addr,
//                                             sizeof(struct sockaddr_in6),
//                                             host, NI_MAXHOST,
//                                             nullptr, 0, NI_NUMERICHOST);
//                    if (result != 0)
//                    {
//                        return false;
//                    }
//                    sockaddr_in6 * address = reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_addr);
//                    localAddress = make_shared<IPV6EndPoint>(address->sin6_addr.__in6_u.__u6_addr8, address->sin6_port,
//                                                             address->sin6_flowinfo, address->sin6_scope_id);
//                    address = reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_netmask);
//                    netmaskaddress = make_shared<IPV6EndPoint>(address->sin6_addr.__in6_u.__u6_addr8, address->sin6_port,
//                                                               address->sin6_flowinfo, address->sin6_scope_id);
//                    if (((currentAdapter->ifa_flags & IFF_BROADCAST) != 0) && (currentAdapter->ifa_ifu.ifu_broadaddr != nullptr))
//                    {
//                        address = reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_ifu.ifu_broadaddr);
//                        broadcastAddress = make_shared<IPV6EndPoint>(address->sin6_addr.__in6_u.__u6_addr8, address->sin6_port,
//                                                                     address->sin6_flowinfo, address->sin6_scope_id);
//                    }
//                    if (((currentAdapter->ifa_flags & IFF_POINTOPOINT) != 0) && (currentAdapter->ifa_ifu.ifu_dstaddr != nullptr))
//                    {
//                        address = reinterpret_cast<sockaddr_in6 *>(currentAdapter->ifa_ifu.ifu_dstaddr);
//                        destAddress = make_shared<IPV6EndPoint>(address->sin6_addr.__in6_u.__u6_addr8, address->sin6_port,
//                                                                address->sin6_flowinfo, address->sin6_scope_id);
//                    }
//                }
//                break;
//            case AF_UNIX:
//                localAddress = make_shared<DomainSocketAddress>(reinterpret_cast<sockaddr_un *>(currentAdapter->ifa_addr)->sun_path);
//                break;
//            case AF_PACKET:
//                sockaddr_ll * address = reinterpret_cast<sockaddr_ll *>(currentAdapter->ifa_addr);
//                localAddress = make_shared<MACAddress>(OSAL::ByteArray(address->sll_addr, address->sll_halen));
//                break;
//        }
//        _adapters.push_back(make_shared<Adapter>(currentAdapter->ifa_name, localAddress, netmaskaddress, broadcastAddress, destAddress, static_cast<AdapterFlags>(currentAdapter->ifa_flags)));
//    }

//    freeifaddrs(adapters);
    return true;
}
