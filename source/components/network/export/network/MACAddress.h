#pragma once

#include "core/ByteArray.h"

namespace Network
{

class MACAddress
{
private:
    Core::ByteArray macAddress;
public:
    static const size_t Size = 6;

    MACAddress() :
        macAddress(Size)
    {
    }
    MACAddress(const MACAddress & other) :
        macAddress(other.macAddress)
    {
    }
    MACAddress(const Core::ByteArray & macAddress) :
        macAddress(Size)
    {
        SetData(macAddress, 0);
    }
    MACAddress(const Core::ByteArray & macAddress, size_t offset) :
        macAddress(Size)
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

    Core::ByteArray GetBytes() const;
    std::string ToString() const;

private:
    void SetData(const Core::ByteArray & data, size_t offset);
    static bool TryParse(const std::string & text, uint8_t & value);
};

inline void PrintTo(const MACAddress & value, std::ostream & stream)
{
    stream << value.ToString();
}

} // namespace Network
