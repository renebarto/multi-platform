#pragma once

#include "osal/ByteArray.h"
#include "osal/NetworkEndPoint.h"

namespace OSAL {
namespace Network {

class OSAL_EXPORT MACAddress : public EndPoint
{
public:
    static const size_t AddressSize = 6;

    MACAddress() :
        _macAddress(AddressSize)
    {
    }
    MACAddress(const MACAddress & other) :
        _macAddress(other._macAddress)
    {
    }
    MACAddress(const ByteArray & macAddress) :
        _macAddress(AddressSize)
    {
        SetData(macAddress, 0);
    }
    MACAddress(const ByteArray & macAddress, size_t offset) :
        _macAddress(AddressSize)
    {
        SetData(macAddress, offset);
    }
    virtual ~MACAddress();
    static MACAddress Parse(const String & text);
    static bool TryParse(const String & text, MACAddress & macAddress);
    MACAddress & operator = (const MACAddress & other);
    bool operator == (const EndPoint & other) const;
    bool operator != (const EndPoint & other) const;
    bool operator == (const MACAddress & other) const;
    bool operator != (const MACAddress & other) const;
    uint8_t operator[] (size_t offset) const;

    virtual SocketFamily Family() const override { return SocketFamily::Packet; }
    virtual size_t Size() const override { return AddressSize; }
    virtual ByteArray GetBytes() const override;
    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    void SetData(const ByteArray & data, size_t offset);
    static bool TryParse(const String & text, uint8_t & value);

    ByteArray _macAddress;
};

} // namespace Network
} // namespace OSAL
