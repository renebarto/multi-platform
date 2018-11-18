#include "osal/bytearray.h"

//#include <algorithm>
//#include <cstring>

using namespace std;

namespace OSAL {

const size_t bytearray::BytesPerRow = 16;
const size_t bytearray::MaxBytesToDisplay = 512;

bytearray bytearray::get(size_t offset, size_t length) const
{
    ASSERT(offset + length <= size());
    if (offset + length > size())
        return bytearray();
    return bytearray(data() + offset, length);
}

uint8_t bytearray::getUInt8(size_t offset) const
{
    ASSERT(offset < size());
    if (offset >= size())
        return 0;
    return _data[offset];
}

uint16_t bytearray::getUInt16(size_t offset) const
{
    return static_cast<uint16_t>(getUInt8(offset) | (getUInt8(offset + 1) << 8));
}

uint32_t bytearray::getUInt32(size_t offset) const
{
    return static_cast<uint32_t>(getUInt16(offset)) | static_cast<uint32_t>(getUInt16(offset + 2) << 16);
}

uint64_t bytearray::getUInt64(size_t offset) const
{
    return (uint64_t)getUInt32(offset) | ((uint64_t)getUInt32(offset + 4) << 32);
}

uint16_t bytearray::getUInt16BE(size_t offset) const
{
    return static_cast<uint16_t>((getUInt8(offset) << 8) | getUInt8(offset + 1));
}

uint32_t bytearray::getUInt32BE(size_t offset) const
{
    return static_cast<uint32_t>((getUInt16BE(offset) << 16)) | static_cast<uint32_t>(getUInt16BE(offset + 2));
}

uint64_t bytearray::getUInt64BE(size_t offset) const
{
    return (static_cast<uint64_t>(getUInt32BE(offset)) << 32) | static_cast<uint64_t>(getUInt32BE(offset + 4));
}

void bytearray::setUInt8(size_t offset, uint8_t value)
{
    if (size() <= offset)
    {
        resize(offset + 1);
    }
    _data[offset] = value;
}

void bytearray::setUInt16(size_t offset, uint16_t value)
{
    setUInt8(offset + 0, static_cast<uint8_t>((value >> 0) & 0xFF));
    setUInt8(offset + 1, static_cast<uint8_t>((value >> 8) & 0xFF));
}

void bytearray::setUInt32(size_t offset, uint32_t value)
{
    setUInt16(offset + 0, static_cast<uint16_t>((value >> 0) & 0xFFFF));
    setUInt16(offset + 2, static_cast<uint16_t>((value >> 16) & 0xFFFF));
}

void bytearray::setUInt64(size_t offset, uint64_t value)
{
    setUInt32(offset + 0, static_cast<uint32_t>((value >> 0) & 0xFFFFFFFF));
    setUInt32(offset + 4, static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF));
}

void bytearray::setUInt16BE(size_t offset, uint16_t value)
{
    setUInt8(offset + 0, static_cast<uint8_t>((value >> 8) & 0xFF));
    setUInt8(offset + 1, static_cast<uint8_t>((value >> 0) & 0xFF));
}

void bytearray::setUInt32BE(size_t offset, uint32_t value)
{
    setUInt16BE(offset + 0, static_cast<uint16_t>((value >> 16) & 0xFFFF));
    setUInt16BE(offset + 2, static_cast<uint16_t>((value >> 0) & 0xFFFF));
}

void bytearray::setUInt64BE(size_t offset, uint64_t value)
{
    setUInt32BE(offset + 0, static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF));
    setUInt32BE(offset + 4, static_cast<uint32_t>((value >> 0) & 0xFFFFFFFF));
}

} // namespace OSAL
