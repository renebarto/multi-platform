#pragma once

#include "osal/NetworkAddress.h"

namespace OSAL
{

class IPV4Address : public OSAL::Network::Address
{
public:
    static IPV4Address Any;
    static IPV4Address None;
    static IPV4Address Broadcast;
    static IPV4Address LocalHost;

    IPV4Address() :
        _ipAddress(None._ipAddress)
    {
    }
    IPV4Address(const IPV4Address & other) :
        _ipAddress(other._ipAddress)
    {
    }
    IPV4Address(const OSAL::ByteArray & ipAddress) :
        _ipAddress(AddressSize)
    {
        SetData(ipAddress, 0);
    }
    IPV4Address(uint32_t ipAddress) :
        _ipAddress(AddressSize)
    {
        SetUInt32(ipAddress);
    }
    IPV4Address(const OSAL::ByteArray & ipAddress, size_t offset) :
        _ipAddress(AddressSize)
    {
        SetData(ipAddress, offset);
    }

    virtual ~IPV4Address();
    static IPV4Address Parse(const std::string & text);
    static bool TryParse(const std::string & text, IPV4Address & ipAddress);
    IPV4Address & operator = (const IPV4Address & other);
    bool operator == (const IPV4Address & other) const;
    bool operator != (const IPV4Address & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    uint32_t GetUInt32() const;
    void SetUInt32(uint32_t value);

    virtual OSAL::Network::SocketFamily Family() const { return OSAL::Network::SocketFamily::InternetV4; }
    virtual size_t Size() const override { return AddressSize; }
    virtual OSAL::ByteArray GetBytes() const override;
    virtual OSAL::String ToString() const override;

private:
    OSAL::ByteArray _ipAddress;
    static const size_t AddressSize = 4;

    void SetData(const OSAL::ByteArray & data, size_t offset);
};

inline void PrintTo(const IPV4Address & value, std::ostream & stream)
{
    stream << value.ToString();
}

} // namespace OSAL

