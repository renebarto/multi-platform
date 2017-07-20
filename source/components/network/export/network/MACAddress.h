#pragma once

#include "core/ByteArray.h"
#include "network/Address.h"

namespace Network
{

class MACAddress : public Address
{
public:
    static const size_t AddressSize = 6;

    MACAddress() :
        macAddress(AddressSize)
    {
    }
    MACAddress(const MACAddress & other) :
        macAddress(other.macAddress)
    {
    }
    MACAddress(const Core::ByteArray & macAddress) :
        macAddress(AddressSize)
    {
        SetData(macAddress, 0);
    }
    MACAddress(const Core::ByteArray & macAddress, size_t offset) :
        macAddress(AddressSize)
    {
        SetData(macAddress, offset);
    }
    virtual ~MACAddress();
    static MACAddress Parse(const std::string & text);
    static bool TryParse(const std::string & text, MACAddress & macAddress);
    MACAddress & operator = (const MACAddress & other);
    bool operator == (const MACAddress & other) const;
    bool operator != (const MACAddress & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    virtual SocketFamily Family() const { return SocketFamily::Packet; }
    virtual size_t Size() const override { return AddressSize; }
    virtual Core::ByteArray GetBytes() const override;
    virtual OSAL::String ToString() const override;

private:
    void SetData(const Core::ByteArray & data, size_t offset);
    static bool TryParse(const OSAL::String & text, uint8_t & value);

    Core::ByteArray macAddress;
};

inline void PrintTo(const MACAddress & value, std::ostream & stream)
{
    stream << value.ToString();
}

} // namespace Network

namespace Core
{

namespace Util
{

bool TryParse(const OSAL::String & text, Network::MACAddress & ipAddress);

} // namespace Util

} // namespace Core

