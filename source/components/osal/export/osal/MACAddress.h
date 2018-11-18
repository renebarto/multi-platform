#pragma once

#include "osal/bytearray.h"
#include "osal/NetworkEndPoint.h"

namespace OSAL {
namespace Network {

class OSAL_EXPORT MACAddress : public EndPoint
{
public:
    static const size_t AddressSize;

    MACAddress()
        : _macAddress(AddressSize)
    {
    }
    MACAddress(const MACAddress & other)
        : _macAddress(other._macAddress)
    {
    }
    MACAddress(const bytearray & macAddress)
        : _macAddress(AddressSize)
    {
        SetData(macAddress, 0);
    }
    MACAddress(const bytearray & macAddress, size_t offset)
        : _macAddress(AddressSize)
    {
        SetData(macAddress, offset);
    }
    virtual ~MACAddress();

    static EndPointPtr Create();
    static EndPointPtr Create(const EndPoint & other);
    static EndPointPtr Create(const std::string & text);

    static MACAddress Parse(const std::string & text);
    static bool TryParse(const std::string & text, MACAddress & macAddress);
    MACAddress & operator = (const MACAddress & other);
    bool operator == (const EndPoint & other) const;
    bool operator != (const EndPoint & other) const;
    bool operator == (const MACAddress & other) const;
    bool operator != (const MACAddress & other) const;
    uint8_t operator[] (size_t offset) const;

    virtual SocketFamily Family() const override { return SocketFamily::Packet; }
    virtual size_t Size() const override { return AddressSize; }
    virtual bytearray GetBytes() const override;
    virtual std::ostream & PrintTo(std::ostream & stream) const override;

private:
    void SetData(const bytearray & data, size_t offset);
    static bool TryParse(const std::string & text, uint8_t & value);

    bytearray _macAddress;
};

} // namespace Network
} // namespace OSAL
