#include "util.h"

#ifdef __linux__
#include <unistd.h>   //_getch
#include <termios.h>  //_getch
#include <sys/socket.h> // socket()
#include <sys/ioctl.h> // ioctl(), SIOCGIFFLAGS
#include <net/if.h> // ifreq
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // inet_ntoa()
#define SOCK_HANDLE int
#define SOCK_INVALID_HANDLE -1
#else
# ifndef _CRT_SECURE_NO_WARNINGS
#  define _CRT_SECURE_NO_WARNINGS 1
# endif
# ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#  define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
# endif
# ifndef _WIN32_WINNT_WINTHRESHOLD
#  define _WIN32_WINNT_WINTHRESHOLD           0x0601
# endif
# ifndef NTDDI_WIN7SP1
#  define NTDDI_WIN7SP1                       0x0601010
# endif
# define SOCK_HANDLE SOCKET
# define SOCK_INVALID_HANDLE SOCKET(INVALID_HANDLE_VALUE)
#include <WinSock2.h>
#include <iphlpapi.h>
#include <iphlpapi.h>
#include <conio.h>
#endif
#include <cstring> // memset(), memcpy(), strncpy()
#include <iostream>
#include <thread>

int GetCh()
{
#ifdef __linux__
    char buf = 0;
    struct termios old = {};
    fflush(stdout);
    if (tcgetattr(0, &old)<0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old)<0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1)<0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old)<0)
        perror("tcsetattr ~ICANON");
    printf("%c\n", buf);
    return buf;
#else
    return _getch();
#endif
}

bool iequals(const std::string& a, const std::string& b)
{
    return std::equal(a.begin(), a.end(),
        b.begin(), b.end(),
        [](char a, char b) {
        return tolower(a) == tolower(b);
    });
}

std::string GetIpAddress(const std::string& interfaceName)
{
    SOCK_HANDLE fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == SOCK_INVALID_HANDLE)
    {
        // error
        std::cerr << "GetIpAddress failed on interface: " << interfaceName << std::endl;
        // SWFATALCHECK(false, RS_LIBCOMMON_GET_LOCALIP_FAILED << "interface:" << interfaceName);
    }

    std::string ipaddress;

#ifdef __linux__
    ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, interfaceName.c_str(), IFNAMSIZ - 1);

    // Wait forever until interface is up.
    while (ioctl(fd, SIOCGIFFLAGS, &ifr) >= 0 && (ifr.ifr_flags & IFF_RUNNING) == 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if ((ifr.ifr_flags & IFF_RUNNING) == 0)
    {
        std::cerr << "GetIpAddress " << interfaceName << " failed" << std::endl;
        // SWFATALCHECK(false, RS_LIBCOMMON_GET_LOCALIP_FAILED << "interface:" << interfaceName);
    }

    while (ipaddress.empty())
    {
        /* Get IP address */
        if (ioctl(fd, SIOCGIFADDR, &ifr) < 0)
        {
            // Not yet
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        else
        {
            // Copy address to prevent aligned warning when casting from sockaddr* to sockaddr_in*
            struct sockaddr_in sai;
            memcpy(&sai, &ifr.ifr_addr, sizeof(sai));
            ipaddress = inet_ntoa(sai.sin_addr);
            //TRACE(CatInformation, interfaceName + " IpAddress:" + ipaddress);
            std::cout << "GetIpAddress " << interfaceName << " IpAddress: " << ipaddress << std::endl;
        }
    }

    if (fd != SOCK_INVALID_HANDLE)
    {
        close(fd);
    }
#else
    ULONG allocSize = 16384;
    unsigned char * buffer{};
    PIP_ADAPTER_ADDRESSES addressInfo{};
    ULONG result{};
    do {
        buffer = new unsigned char[allocSize];
        addressInfo = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buffer);
        ULONG addressInfoSize = allocSize;
        result = GetAdaptersAddresses(
            AF_INET,
            GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_INCLUDE_ALL_INTERFACES,
            nullptr,
            addressInfo,
            &addressInfoSize);
        if (result == ERROR_BUFFER_OVERFLOW) {
            delete[] buffer;
            addressInfo = nullptr;
            allocSize *= 2;
        }
    } while (result == ERROR_BUFFER_OVERFLOW);
    if (addressInfo != nullptr) {
        PIP_ADAPTER_ADDRESSES currentAddress = addressInfo;
        while (currentAddress) {
            printf("\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n", currentAddress->Length);
            printf("\tIfIndex (IPv4 interface): %lu\n", currentAddress->IfIndex);
            printf("\tAdapter name: %s\n", currentAddress->AdapterName);
            std::wstring wideString = currentAddress->FriendlyName;
            std::string narrowString(wideString.begin(), wideString.end());
            bool matchAdapter = iequals(narrowString, interfaceName);

            auto unicastAddress = currentAddress->FirstUnicastAddress;
            if (unicastAddress != NULL) {
                int count;
                for (count = 0; unicastAddress != NULL; count++) {
                    SOCKADDR_IN * sockAddr = reinterpret_cast<SOCKADDR_IN *>(unicastAddress->Address.lpSockaddr);
                    BYTE * macAddress = currentAddress->PhysicalAddress;
                    BYTE netmaskLength = unicastAddress->OnLinkPrefixLength;
                    IN_ADDR netmask{};
                    std::string netmaskString = inet_ntoa(netmask);
                    std::string ipAddressString = inet_ntoa(sockAddr->sin_addr);
                    ConvertLengthToIpv4Mask(netmaskLength, reinterpret_cast<ULONG *>(&netmask));
                    printf("\tUnicast Address: %s:%d Netmask: %s MAC: %02x-%02x-%02x-%02x-%02x-%02x\n", ipAddressString.c_str(), sockAddr->sin_port,
                        netmaskString.c_str(), macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);
                    if (ipaddress.empty() && matchAdapter) {
                        ipaddress = ipAddressString;
                    }

                    unicastAddress = unicastAddress->Next;
                }
                printf("\tNumber of Unicast Addresses: %d\n", count);
            }
            else
                printf("\tNo Unicast Addresses\n");

            auto anycastAddress = currentAddress->FirstAnycastAddress;
            if (anycastAddress) {
                int count;
                for (count = 0; anycastAddress != NULL; count++) {
                    SOCKADDR_IN * sockAddr = reinterpret_cast<SOCKADDR_IN *>(anycastAddress->Address.lpSockaddr);
                    BYTE * macAddress = currentAddress->PhysicalAddress;
                    printf("\tAnycast Address: %s:%d MAC: %02x-%02x-%02x-%02x-%02x-%02x\n", inet_ntoa(sockAddr->sin_addr), sockAddr->sin_port,
                        macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);

                    anycastAddress = anycastAddress->Next;
                }
                printf("\tNumber of Anycast Addresses: %d\n", count);
            }
            else
                printf("\tNo Anycast Addresses\n");

            auto multicastAddress = currentAddress->FirstMulticastAddress;
            if (multicastAddress) {
                int count;
                for (count = 0; multicastAddress != NULL; count++) {
                    SOCKADDR_IN * sockAddr = reinterpret_cast<SOCKADDR_IN *>(multicastAddress->Address.lpSockaddr);
                    BYTE * macAddress = currentAddress->PhysicalAddress;
                    printf("\tMulticast Address: %s:%d MAC: %02x-%02x-%02x-%02x-%02x-%02x\n", inet_ntoa(sockAddr->sin_addr), sockAddr->sin_port,
                        macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);

                    multicastAddress = multicastAddress->Next;
                }
                printf("\tNumber of Multicast Addresses: %d\n", count);
            }
            else
                printf("\tNo Multicast Addresses\n");

            auto dnsServerAddress = currentAddress->FirstDnsServerAddress;
            if (dnsServerAddress) {
                int count;
                for (count = 0; dnsServerAddress != NULL; count++) {
                    SOCKADDR_IN * sockAddr = reinterpret_cast<SOCKADDR_IN *>(dnsServerAddress->Address.lpSockaddr);
                    BYTE * macAddress = currentAddress->PhysicalAddress;
                    printf("\tDNS Server Address: %s:%d MAC: %02x-%02x-%02x-%02x-%02x-%02x\n", inet_ntoa(sockAddr->sin_addr), sockAddr->sin_port,
                        macAddress[0], macAddress[1], macAddress[2], macAddress[3], macAddress[4], macAddress[5]);

                    dnsServerAddress = dnsServerAddress->Next;
                }
                printf("\tNumber of DNS Server Addresses: %d\n", count);
            }
            else
                printf("\tNo DNS Server Addresses\n");

            printf("\tDNS Suffix: %S\n", currentAddress->DnsSuffix);
            printf("\tDescription: %S\n", currentAddress->Description);
            printf("\tFriendly name: %S\n", currentAddress->FriendlyName);

            if (currentAddress->PhysicalAddressLength != 0) {
                printf("\tPhysical address: ");
                for (unsigned i = 0; i < currentAddress->PhysicalAddressLength;
                    i++) {
                    if (i == (currentAddress->PhysicalAddressLength - 1))
                        printf("%.2X\n",
                        (int)currentAddress->PhysicalAddress[i]);
                    else
                        printf("%.2X-",
                        (int)currentAddress->PhysicalAddress[i]);
                }
            }
            printf("\tFlags: %ld\n", currentAddress->Flags);
            printf("\tMtu: %lu\n", currentAddress->Mtu);
            printf("\tIfType: %ld\n", currentAddress->IfType);
            printf("\tOperStatus: %d\n", static_cast<int>(currentAddress->OperStatus));
            printf("\tIpv6IfIndex (IPv6 interface): %lu\n", currentAddress->Ipv6IfIndex);
            printf("\tZoneIndices (hex): ");
            for (int i = 0; i < 16; i++)
                printf("%lx ", currentAddress->ZoneIndices[i]);
            printf("\n");

#if defined(WIN_MSVC)
            printf("\tTransmit link speed: %I64u\n", currentAddress->TransmitLinkSpeed);
            printf("\tReceive link speed: %I64u\n", currentAddress->ReceiveLinkSpeed);
#else
            printf("\tTransmit link speed: %llu\n", currentAddress->TransmitLinkSpeed);
            printf("\tReceive link speed: %llu\n", currentAddress->ReceiveLinkSpeed);
#endif

            auto prefix = currentAddress->FirstPrefix;
            if (prefix) {
                int count;
                for (count = 0; prefix != NULL; count++)
                    prefix = prefix->Next;
                printf("\tNumber of IP Adapter Prefix entries: %d\n", count);
            }
            else
                printf("\tNumber of IP Adapter Prefix entries: 0\n");

            printf("\n");

            currentAddress = currentAddress->Next;
        }
    }
    if (fd != SOCK_INVALID_HANDLE)
    {
        closesocket(fd);
    }
    delete[] buffer;

#endif

    return ipaddress;
}
