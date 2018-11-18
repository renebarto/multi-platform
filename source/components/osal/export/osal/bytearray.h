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
    // These constants are purposely made declspec(dllexport), as otherwise the linker will fail to find them.
    // Making the complete class declspec(dllexport) is NOT a solution, this will enforce to export every 
    // instantiation of array<uint8_t>, leading to multiple symbols linker errors.
    // The current solution however generates a linker warning 4217: warning LNK4217: locally defined symbol s imported in function f, which is therefore ignored.
    static const OSAL_EXPORT size_t BytesPerRow;
    static const OSAL_EXPORT size_t MaxBytesToDisplay;

    bytearray()
        : array()
    {}
    bytearray(size_t size)
        : array(size)
    {}
    bytearray(const uint8_t * data, size_t length)
        : array(data, length)
    {}
    bytearray(std::initializer_list<uint8_t> data)
        : array(data)
    {}
    bytearray(const char * data, size_t length)
        : array()
    {
        set(0, reinterpret_cast<const uint8_t *>(data), length);
    }
    explicit bytearray(const char * data)
        : array()
    {
        set(0, reinterpret_cast<const uint8_t *>(data), strlen(data));
    }
    bytearray(std::string data)
        : array()
    {
        set(0, reinterpret_cast<const uint8_t *>(data.c_str()), data.length());
    }

    bytearray(const bytearray & other)
        : array(other)
    {}
    bytearray(bytearray && other)
        : array(std::move(other))
    {}

    bytearray & operator = (const bytearray & other)
    {
        if (this != &other)
        {
            array::operator =(other);
        }

        return *this;
    }

    bytearray & operator = (bytearray && other)
    {
        if (this != &other)
        {
            array::operator =(std::move(other));
        }

        return *this;
    }

    OSAL_EXPORT uint8_t getUInt8(size_t offset) const;
    OSAL_EXPORT uint16_t getUInt16(size_t offset) const;
    OSAL_EXPORT uint32_t getUInt32(size_t offset) const;
    OSAL_EXPORT uint64_t getUInt64(size_t offset) const;
    OSAL_EXPORT uint16_t getUInt16BE(size_t offset) const;
    OSAL_EXPORT uint32_t getUInt32BE(size_t offset) const;
    OSAL_EXPORT uint64_t getUInt64BE(size_t offset) const;
    size_t get(size_t offset, uint8_t * data, size_t length) const
    {
        return array::get(offset, data, length);
    }
    size_t get(size_t offset, bytearray & data, size_t length) const
    {
        return array::get(offset, data, length);
    }
    OSAL_EXPORT bytearray get(size_t offset, size_t length) const;
    OSAL_EXPORT void setUInt8(size_t offset, uint8_t value);
    OSAL_EXPORT void setUInt16(size_t offset, uint16_t value);
    OSAL_EXPORT void setUInt32(size_t offset, uint32_t value);
    OSAL_EXPORT void setUInt64(size_t offset, uint64_t value);
    OSAL_EXPORT void setUInt16BE(size_t offset, uint16_t value);
    OSAL_EXPORT void setUInt32BE(size_t offset, uint32_t value);
    OSAL_EXPORT void setUInt64BE(size_t offset, uint64_t value);
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

inline bool operator == (const OSAL::bytearray & lhs, const OSAL::bytearray & rhs)
{
    return lhs.Equals(rhs);
}

inline bool operator != (const OSAL::bytearray & lhs, const OSAL::bytearray & rhs)
{
    return !lhs.Equals(rhs);
}

