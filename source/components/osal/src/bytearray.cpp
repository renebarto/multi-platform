#include "osal/windows/bytearray-new.h"

//#include <algorithm>
//#include <cstring>

using namespace std;

namespace OSAL {

bytearray::bytearray(const char * data, size_t length)
    : array()
{
    set(0, reinterpret_cast<const uint8_t *>(data), length);
}

bytearray::bytearray(const char * data)
    : array()
{
    set(0, reinterpret_cast<const uint8_t *>(data), strlen(data));
}

bytearray::bytearray(std::string data)
    : array()
{
    set(0, reinterpret_cast<const uint8_t *>(data.c_str()), data.length());
}

bytearray & bytearray::operator = (const bytearray & other)
{
    if (this != &other)
    {
        array::operator =(other);
    }

    return *this;
}

bytearray & bytearray::operator = (bytearray && other)
{
    if (this != &other)
    {
        array::operator =(std::move(other));
    }

    return *this;
}

bytearray bytearray::Get(size_t offset, size_t length) const
{
    ASSERT(offset + length <= size());
    if (offset + length > size())
        return bytearray();
    return bytearray(data() + offset, length);
}

uint8_t bytearray::GetUInt8(size_t offset) const
{
    ASSERT(offset < size());
    if (offset >= size())
        return 0;
    return _data[offset];
}

uint16_t bytearray::GetUInt16(size_t offset) const
{
    return static_cast<uint16_t>(GetUInt8(offset) | (GetUInt8(offset + 1) << 8));
}

uint32_t bytearray::GetUInt32(size_t offset) const
{
    return static_cast<uint32_t>(GetUInt16(offset)) | static_cast<uint32_t>(GetUInt16(offset + 2) << 16);
}

uint64_t bytearray::GetUInt64(size_t offset) const
{
    return (uint64_t)GetUInt32(offset) | ((uint64_t)GetUInt32(offset + 4) << 32);
}

uint16_t bytearray::GetUInt16BE(size_t offset) const
{
    return static_cast<uint16_t>((GetUInt8(offset) << 8) | GetUInt8(offset + 1));
}

uint32_t bytearray::GetUInt32BE(size_t offset) const
{
    return static_cast<uint32_t>((GetUInt16BE(offset) << 16)) | static_cast<uint32_t>(GetUInt16BE(offset + 2));
}

uint64_t bytearray::GetUInt64BE(size_t offset) const
{
    return (static_cast<uint64_t>(GetUInt32BE(offset)) << 32) | static_cast<uint64_t>(GetUInt32BE(offset + 4));
}

void bytearray::SetUInt8(size_t offset, uint8_t value)
{
    if (size() <= offset)
    {
        resize(offset + 1);
    }
    _data[offset] = value;
}

void bytearray::SetUInt16(size_t offset, uint16_t value)
{
    SetUInt8(offset + 0, static_cast<uint8_t>((value >> 0) & 0xFF));
    SetUInt8(offset + 1, static_cast<uint8_t>((value >> 8) & 0xFF));
}

void bytearray::SetUInt32(size_t offset, uint32_t value)
{
    SetUInt16(offset + 0, static_cast<uint16_t>((value >> 0) & 0xFFFF));
    SetUInt16(offset + 2, static_cast<uint16_t>((value >> 16) & 0xFFFF));
}

void bytearray::SetUInt64(size_t offset, uint64_t value)
{
    SetUInt32(offset + 0, static_cast<uint32_t>((value >> 0) & 0xFFFFFFFF));
    SetUInt32(offset + 4, static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF));
}

void bytearray::SetUInt16BE(size_t offset, uint16_t value)
{
    SetUInt8(offset + 0, static_cast<uint8_t>((value >> 8) & 0xFF));
    SetUInt8(offset + 1, static_cast<uint8_t>((value >> 0) & 0xFF));
}

void bytearray::SetUInt32BE(size_t offset, uint32_t value)
{
    SetUInt16BE(offset + 0, static_cast<uint16_t>((value >> 16) & 0xFFFF));
    SetUInt16BE(offset + 2, static_cast<uint16_t>((value >> 0) & 0xFFFF));
}

void bytearray::SetUInt64BE(size_t offset, uint64_t value)
{
    SetUInt32BE(offset + 0, static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF));
    SetUInt32BE(offset + 4, static_cast<uint32_t>((value >> 0) & 0xFFFFFFFF));
}

} // namespace OSAL
