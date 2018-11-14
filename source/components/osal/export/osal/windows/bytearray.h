#pragma once

#include <cstdint>
#include <initializer_list>
#include <streambuf>
#include <string>
#include "osal/array.h"

namespace OSAL
{

class bytearray : public array<uint8_t>
{
public:
    static const size_t BytesPerRow = 16;
    static const size_t MaxBytesToDisplay = 512;

    bytearray() : array() {}
    bytearray(size_t size) : array(size) {}
    bytearray(const uint8_t * data, size_t length) : array(data, length) {}
    OSAL_EXPORT bytearray(const char * data, size_t length);
    explicit OSAL_EXPORT bytearray(const char * data);
    OSAL_EXPORT bytearray(std::string data);
    bytearray(const bytearray & other) : array(other) {}
    bytearray(bytearray && other)
        : array(std::move(other))
    {
    }
    bytearray(std::initializer_list<uint8_t> data) : array(data) {}

    OSAL_EXPORT uint8_t GetUInt8(size_t offset) const;
    OSAL_EXPORT uint16_t GetUInt16(size_t offset) const;
    OSAL_EXPORT uint32_t GetUInt32(size_t offset) const;
    OSAL_EXPORT uint64_t GetUInt64(size_t offset) const;
    OSAL_EXPORT uint16_t GetUInt16BE(size_t offset) const;
    OSAL_EXPORT uint32_t GetUInt32BE(size_t offset) const;
    OSAL_EXPORT uint64_t GetUInt64BE(size_t offset) const;
    //size_t Get(size_t offset, uint8_t * data, size_t length) const
    //{
    //    return array::Get(offset, data, length);
    //}
    size_t Get(size_t offset, bytearray & data, size_t length) const
    {
        return array::get(offset, data, length);
    }
    OSAL_EXPORT bytearray Get(size_t offset, size_t length) const;
    OSAL_EXPORT void SetUInt8(size_t offset, uint8_t value);
    OSAL_EXPORT void SetUInt16(size_t offset, uint16_t value);
    OSAL_EXPORT void SetUInt32(size_t offset, uint32_t value);
    OSAL_EXPORT void SetUInt64(size_t offset, uint64_t value);
    OSAL_EXPORT void SetUInt16BE(size_t offset, uint16_t value);
    OSAL_EXPORT void SetUInt32BE(size_t offset, uint32_t value);
    OSAL_EXPORT void SetUInt64BE(size_t offset, uint64_t value);
    //void Append(const bytearray & data)
    //{
    //    Set(this->Size(), data);
    //}
    OSAL_EXPORT bytearray & operator = (const bytearray & other);
    OSAL_EXPORT bytearray & operator = (bytearray && other);
    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const
    {
        return s << *this;
    }
};

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s, const bytearray & value)
{
    return value.PrintTo(s);
}

template <class Elem, class Traits>
std::basic_ostream<Elem, Traits> & operator<<(std::basic_ostream<Elem, Traits> &s, const bytearray & value)
{
    s << OSAL::System::TypeName(value) << " Size: " << value.size() << std::endl;
    size_t maxValuesToDisplay = std::min<size_t>(value.size(), bytearray::MaxBytesToDisplay);
    for (size_t offset = 0; offset < maxValuesToDisplay; offset += bytearray::BytesPerRow)
    {
        for (size_t i = 0; i < bytearray::BytesPerRow; i++)
        {
            if (i + offset < maxValuesToDisplay)
            {
                uint8_t byteValue = value[i + offset];
                s << std::hex << std::setw(2) << std::setfill('0') << (int)byteValue << " ";
            }
            else
            {
                s << "   ";
            }
        }
        for (size_t i = 0; i < bytearray::BytesPerRow; i++)
        {
            if (i + offset < maxValuesToDisplay)
            {
                uint8_t byteValue = value[i + offset];
                s << static_cast<char>(((byteValue >= 32) && (byteValue < 128)) ? byteValue : '?') << " ";
            }
        }
        s << std::endl;
    }
    s << std::endl << std::flush;
    return s;
}

} // namespace OSAL

