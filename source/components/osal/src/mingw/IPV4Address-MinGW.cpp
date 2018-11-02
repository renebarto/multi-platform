#include "osal/IPV4Address.h"

#include <sstream>
#include "osal/osal.h"
#include "osal/Assert.h"
#include "osal/Exception.h"

using namespace std;
using namespace OSAL;
using namespace Network;

IPV4Address const IPV4Address::None = IPV4Address({0, 0, 0, 0});
IPV4Address const IPV4Address::Any = IPV4Address({0, 0, 0, 0});
IPV4Address const IPV4Address::Broadcast = IPV4Address({255, 255, 255, 255});
IPV4Address const IPV4Address::LocalHost = IPV4Address({127, 0, 0, 1});

// Todo: merge this with other implementations
static int inet_pton(int af, const char *src, void *dst)
{
    struct sockaddr_storage ss;
    int size = sizeof(ss);
    char src_copy[INET6_ADDRSTRLEN+1];

    ZeroMemory(&ss, sizeof(ss));
    /* stupid non-const API */
    strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
    src_copy[INET6_ADDRSTRLEN] = 0;

    if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
        switch(af) {
            case AF_INET:
                *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
                return 1;
            case AF_INET6:
                *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
                return 1;
        }
    }
    return 0;
}

// Todo: merge this with other implementations
//static const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
//{
//    struct sockaddr_storage ss;
//    unsigned long s = size;
//
//    ZeroMemory(&ss, sizeof(ss));
//    ss.ss_family = af;
//
//    switch(af) {
//        case AF_INET:
//            ((struct sockaddr_in *)&ss)->sin_addr = *(struct in_addr *)src;
//            break;
//        case AF_INET6:
//            ((struct sockaddr_in6 *)&ss)->sin6_addr = *(struct in6_addr *)src;
//            break;
//        default:
//            return NULL;
//    }
//    /* cannot direclty use &size because of strict aliasing rules */
//    return (WSAAddressToString((struct sockaddr *)&ss, sizeof(ss), NULL, dst, &s) == 0)?
//           dst : NULL;
//}

IPV4Address::~IPV4Address()
{
}

IPV4Address IPV4Address::Parse(const string & text)
{
    IPV4Address ipAddress;
    if (!TryParse(text, ipAddress))
    {
        ostringstream stream;
        stream << "IPV4Address string representation must be formatted as ddd.ddd.ddd.ddd, string is " << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, "text", stream.str());
    }
    return ipAddress;
}

bool IPV4Address::TryParse(const string & text, IPV4Address & ipAddress)
{
    in_addr address;
    int errorCode = inet_pton(AF_INET, text.c_str(), &address);
    if (errorCode == 0)
    {
        addrinfo * addressInfo;
        addrinfo hints = { 0, AF_INET, 0, 0, 0, nullptr, nullptr, nullptr };
        errorCode = getaddrinfo(text.c_str(), nullptr, &hints, &addressInfo);
        if (errorCode != 0)
            return false;
        address = ((sockaddr_in *)(addressInfo->ai_addr))->sin_addr;
        freeaddrinfo(addressInfo);
    }
    ipAddress = IPV4Address((uint32_t)address.s_addr);
    return true;
}

IPV4Address & IPV4Address::operator = (const IPV4Address & other)
{
    _ipAddress = other._ipAddress;
    return *this;
}

bool IPV4Address::operator == (const IPV4Address & other) const
{
    if (&other == this)
        return true;
    return (other._ipAddress == _ipAddress);
}

bool IPV4Address::operator != (const IPV4Address & other) const
{
    return ! this->operator ==(other);
}

uint8_t & IPV4Address::operator[] (size_t offset)
{
    ASSERT(offset < AddressSize);
    return _ipAddress[offset];
}

uint32_t IPV4Address::GetUInt32() const
{
    return _ipAddress.GetUInt32(0);
}

void IPV4Address::SetUInt32(uint32_t value)
{
    _ipAddress.SetUInt32(0, value);
}

OSAL::ByteArray IPV4Address::GetBytes() const
{
    return _ipAddress;
}

std::ostream & IPV4Address::PrintTo(std::ostream & stream) const
{
    stream << (int)_ipAddress[size_t{0}] << ".";
    stream << (int)_ipAddress[size_t{1}] << ".";
    stream << (int)_ipAddress[size_t{2}] << ".";
    stream << (int)_ipAddress[size_t{3}];
    return stream;
}

void IPV4Address::SetData(const OSAL::ByteArray & data, size_t offset)
{
    ASSERT(offset + AddressSize <= data.Size());
    _ipAddress.Set(0, data.Data() + offset, AddressSize);
}
