#pragma once

#include <cstdint>
#include <initializer_list>
#include <streambuf>
#include <string>
#include "osal/Array.h"

namespace OSAL
{

class ByteArray : public Array<uint8_t>
{
public:
    ByteArray() : Array() {}
    ByteArray(size_t size) : Array(size) {}
    ByteArray(const uint8_t * data, size_t length) : Array(data, length) {}
    ByteArray(const char * data, size_t length);
    explicit ByteArray(const char * data);
    ByteArray(std::string data);
    ByteArray(const ByteArray & other) : Array(other) {}
    ByteArray(ByteArray && other) : Array()
    {
        Move(std::move(other));
    }
    ByteArray(std::initializer_list<uint8_t> data) : Array(data) {}

    uint8_t GetUInt8(size_t offset) const;
    uint16_t GetUInt16(size_t offset) const;
    uint32_t GetUInt32(size_t offset) const;
    uint64_t GetUInt64(size_t offset) const;
    uint16_t GetUInt16BE(size_t offset) const;
    uint32_t GetUInt32BE(size_t offset) const;
    uint64_t GetUInt64BE(size_t offset) const;
    size_t Get(size_t offset, uint8_t * data, size_t length) const
    {
        return Array::Get(offset, data, length);
    }
    size_t Get(size_t offset, ByteArray & data, size_t length) const
    {
        return Array::Get(offset, data, length);
    }
    ByteArray Get(size_t offset, size_t length) const;
    void SetUInt8(size_t offset, uint8_t value);
    void SetUInt16(size_t offset, uint16_t value);
    void SetUInt32(size_t offset, uint32_t value);
    void SetUInt64(size_t offset, uint64_t value);
    void SetUInt16BE(size_t offset, uint16_t value);
    void SetUInt32BE(size_t offset, uint32_t value);
    void SetUInt64BE(size_t offset, uint64_t value);
    void Append(const ByteArray & data)
    {
        Set(this->Size(), data);
    }
    ByteArray & operator = (const ByteArray & other);
    ByteArray & operator = (ByteArray && other);
    std::ostream & PrintTo(std::ostream & stream) const override;
};

inline void PrintTo(const ByteArray & value, std::ostream & stream)
{
    value.PrintTo(stream);
}

inline std::ostream & operator << (std::ostream & stream, const ByteArray & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace OSAL

