#include "osal/IPV6Address.h"
#include <sstream>
#include "osal/OSAL.h"

using namespace std;
using namespace OSAL;
using namespace Network;

IPV6Address IPV6Address::None = IPV6Address({0, 0, 0, 0, 0, 0, 0, 0,
                                             0, 0, 0, 0, 0, 0, 0, 0, });
IPV6Address IPV6Address::Any = IPV6Address({0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0, });
IPV6Address IPV6Address::Broadcast = IPV6Address({255, 255, 255, 255, 255, 255, 255, 255,
                                                  255, 255, 255, 255, 255, 255, 255, 255, });
IPV6Address IPV6Address::LocalHost = IPV6Address({0, 0, 0, 0, 0, 0, 0, 0,
                                                  0, 0, 0, 0, 0, 0, 0, 1});

IPV6Address::~IPV6Address()
{
}

IPV6Address IPV6Address::Parse(const OSAL::String & text)
{
    IPV6Address ipAddress;
    if (!TryParse(text, ipAddress))
    {
        basic_ostringstream<OSAL::Char> stream;
        stream << _("IPV6Address string representation must be formatted as xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx ") << endl
               << _("(or a shortened format), string is ") << text;
        throw OSAL::ArgumentException(__func__, __FILE__, __LINE__, _("text"), stream.str());
    }
    return ipAddress;
}

bool IPV6Address::TryParse(const OSAL::String & text, IPV6Address & ipAddress)
{
    in6_addr address;
    std::string narrowText = ToNarrowString(text);
    int errorCode = inet_pton(AF_INET6, narrowText.c_str(), &address);
    if (errorCode == 0)
    {
        // Weird, when specifying "localhost", the hostname does not (always) resolve
        // So create a hack to return the local address in this case.
        if (OSAL::Strings::strcasecmp(text.c_str(), _("localhost"), text.length()) == 0)
        {
            uint8_t localAddress[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
            memcpy(address.u.Byte, localAddress, sizeof(localAddress));
        }
        else
        {
            addrinfo * addressInfo;
            addrinfo hints = { 0, AF_INET6, 0, 0, 0, nullptr, nullptr, nullptr };
            errorCode = getaddrinfo(narrowText.c_str(), nullptr, &hints, &addressInfo);
            if (errorCode != 0)
                return false;
            address = ((sockaddr_in6 *)(addressInfo->ai_addr))->sin6_addr;
            freeaddrinfo(addressInfo);
        }
    }
    ipAddress = IPV6Address(address.u.Byte);
    return true;
}

IPV6Address & IPV6Address::operator = (const IPV6Address & other)
{
    _ipAddress = other._ipAddress;
    return *this;
}

bool IPV6Address::operator == (const IPV6Address & other) const
{
    if (&other == this)
        return true;
    return (other._ipAddress == _ipAddress);
}

bool IPV6Address::operator != (const IPV6Address & other) const
{
    return ! this->operator ==(other);
}

uint8_t & IPV6Address::operator[] (size_t offset)
{
    assert(offset < AddressSize);
    return _ipAddress[offset];
}

const uint8_t & IPV6Address::operator[] (size_t offset) const
{
    assert(offset < AddressSize);
    return _ipAddress[offset];
}

OSAL::ByteArray IPV6Address::GetData() const
{
    return _ipAddress;
}

void IPV6Address::SetData(const OSAL::ByteArray & value)
{
    _ipAddress.Set(0, value);
}

OSAL::ByteArray IPV6Address::GetBytes() const
{
    return _ipAddress;
}

OSAL::String IPV6Address::ToString() const
{
    basic_ostringstream<OSAL::Char> stream;
    static const size_t NumWords = AddressSize / 2;
    uint16_t words[NumWords];
    size_t zeroSequenceStartMax = 0;
    size_t zeroSequenceLengthMax = 0;
    size_t zeroSequenceStart = 0;
    size_t zeroSequenceLength = 0;
    bool inZeroSequence = false;
    for (size_t i = 0; i < NumWords; ++i)
    {
        words[i] = _ipAddress.GetUInt16BE(i * 2);
        if (words[i] == 0)
        {
            if (!inZeroSequence)
            {
                zeroSequenceStart = i;
                inZeroSequence = true;
            }
        }
        else
        {
            if (inZeroSequence)
            {
                zeroSequenceLength = i - zeroSequenceStart;
                inZeroSequence = false;
                if (zeroSequenceLength > zeroSequenceLengthMax)
                {
                    zeroSequenceStartMax = zeroSequenceStart;
                    zeroSequenceLengthMax = zeroSequenceLength;
                }
                zeroSequenceStart = static_cast<size_t>(-1);
                zeroSequenceLength = 0;
            }
        }
    }
    if (inZeroSequence)
    {
        zeroSequenceLength = NumWords - zeroSequenceStart;
        if (zeroSequenceLength > zeroSequenceLengthMax)
        {
            zeroSequenceStartMax = zeroSequenceStart;
            zeroSequenceLengthMax = zeroSequenceLength;
        }

    }
    stream << hex;
    for (size_t i = 0; i < NumWords; ++i)
    {
        if ((i == zeroSequenceStartMax) && (zeroSequenceLengthMax > 1))
        {
            stream << _("::");
        }
        else if ((i < zeroSequenceStartMax) || (i >= zeroSequenceStartMax + zeroSequenceLengthMax))
        {
            stream << words[i];
            if (((i + 1) < NumWords) && ((i + 1) != zeroSequenceStartMax))
                stream << _(":");
        }
    }
    stream << dec;
    return stream.str();
}

void IPV6Address::SetData(const OSAL::ByteArray & data, size_t offset)
{
    assert(offset + AddressSize <= data.Size());
    _ipAddress.Set(0, data.Data() + offset, AddressSize);
}
