#pragma once

#include "osal/ByteArray.h"
#include "osal/NetworkAddress.h"

namespace OSAL {
namespace Network {

class MACAddress : public OSAL::Network::Address
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
    MACAddress(const OSAL::ByteArray & macAddress) :
        macAddress(AddressSize)
    {
        SetData(macAddress, 0);
    }
    MACAddress(const OSAL::ByteArray & macAddress, size_t offset) :
        macAddress(AddressSize)
    {
        SetData(macAddress, offset);
    }
    virtual ~MACAddress();
    static MACAddress Parse(const OSAL::String & text);
    static bool TryParse(const OSAL::String & text, MACAddress & macAddress);
    MACAddress & operator = (const MACAddress & other);
    bool operator == (const Address & other) const;
    bool operator != (const Address & other) const;
    bool operator == (const MACAddress & other) const;
    bool operator != (const MACAddress & other) const;
    uint8_t & operator[] (size_t offset);
    const uint8_t & operator[] (size_t offset) const;

    virtual OSAL::Network::SocketFamily Family() const { return OSAL::Network::SocketFamily::Packet; }
    virtual size_t Size() const override { return AddressSize; }
    virtual OSAL::ByteArray GetBytes() const override;
    virtual OSAL::String ToString() const override;

private:
    void SetData(const OSAL::ByteArray & data, size_t offset);
    static bool TryParse(const OSAL::String & text, uint8_t & value);

    OSAL::ByteArray macAddress;
};

inline void PrintTo(const MACAddress & value, std::basic_ostream<OSAL::Char> & stream)
{
    stream << value.ToString();
}

} // namespace Network
} // namespace OSAL
