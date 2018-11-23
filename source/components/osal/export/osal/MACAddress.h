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
    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & Print(std::basic_ostream<Elem, Traits> & s) const;
    virtual std::basic_ostream<char, std::char_traits<char>> & PrintTo(std::basic_ostream<char, std::char_traits<char>> & s) const override { return Print(s); }
    virtual std::basic_ostream<wchar_t, std::char_traits<wchar_t>> & PrintTo(std::basic_ostream<wchar_t, std::char_traits<wchar_t>> & s) const override { return Print(s); }

private:
    void SetData(const bytearray & data, size_t offset);
    static bool TryParse(const std::string & text, uint8_t & value);

    bytearray _macAddress;
};

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & MACAddress::Print(std::basic_ostream<Elem, Traits> & s) const
{
    s << std::hex << std::setfill(Elem('0'));
    s << std::uppercase << std::setw(2) << (int)_macAddress[size_t{0}] << "-";
    s << std::uppercase << std::setw(2) << (int)_macAddress[size_t{1}] << "-";
    s << std::uppercase << std::setw(2) << (int)_macAddress[size_t{2}] << "-";
    s << std::uppercase << std::setw(2) << (int)_macAddress[size_t{3}] << "-";
    s << std::uppercase << std::setw(2) << (int)_macAddress[size_t{4}] << "-";
    s << std::uppercase << std::setw(2) << (int)_macAddress[size_t{5}];
    return s;
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s, const MACAddress & value)
{
    return value.PrintTo(s);
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & operator<<(std::basic_ostream<Elem, Traits> &s, const MACAddress & value)
{
    return value.PrintTo(s);
}

} // namespace Network
} // namespace OSAL
