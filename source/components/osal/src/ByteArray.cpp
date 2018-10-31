#include "osal/ByteArray.h"

//#include <algorithm>
//#include <cstring>

using namespace std;

namespace OSAL {

static const size_t BytesPerRow = 16;
static const size_t MaxBytesToDisplay = 512;

ByteArray::ByteArray(const char * data, size_t length)
    : Array()
{
    Set(0, reinterpret_cast<const uint8_t *>(data), length);
}

ByteArray::ByteArray(const char * data)
    : Array()
{
    Set(0, reinterpret_cast<const uint8_t *>(data), strlen(data));
}

ByteArray::ByteArray(std::string data)
    : Array()
{
    Set(0, reinterpret_cast<const uint8_t *>(data.c_str()), data.length());
}

ByteArray & ByteArray::operator = (const ByteArray & other)
{
    if (this != &other)
    {
        Copy(other);
    }

    return *this;
}

ByteArray & ByteArray::operator = (ByteArray && other)
{
    if (this != &other)
    {
        Move(std::move(other));
    }

    return *this;
}

ByteArray ByteArray::Get(size_t offset, size_t length) const
{
    ASSERT(offset + length <= _size);
    if (offset + length > _size)
        return ByteArray();
    return ByteArray(Data() + offset, length);
}

uint8_t ByteArray::GetUInt8(size_t offset) const
{
    ASSERT(offset < _size);
    if (offset >= _size)
        return 0;
    return _data[offset];
}

uint16_t ByteArray::GetUInt16(size_t offset) const
{
    return static_cast<uint16_t>(GetUInt8(offset) | (GetUInt8(offset + 1) << 8));
}

uint32_t ByteArray::GetUInt32(size_t offset) const
{
    return static_cast<uint32_t>(GetUInt16(offset)) | static_cast<uint32_t>(GetUInt16(offset + 2) << 16);
}

uint64_t ByteArray::GetUInt64(size_t offset) const
{
    return (uint64_t)GetUInt32(offset) | ((uint64_t)GetUInt32(offset + 4) << 32);
}

uint16_t ByteArray::GetUInt16BE(size_t offset) const
{
    return static_cast<uint16_t>((GetUInt8(offset) << 8) | GetUInt8(offset + 1));
}

uint32_t ByteArray::GetUInt32BE(size_t offset) const
{
    return static_cast<uint32_t>((GetUInt16BE(offset) << 16)) | static_cast<uint32_t>(GetUInt16BE(offset + 2));
}

uint64_t ByteArray::GetUInt64BE(size_t offset) const
{
    return (static_cast<uint64_t>(GetUInt32BE(offset)) << 32) | static_cast<uint64_t>(GetUInt32BE(offset + 4));
}

void ByteArray::SetUInt8(size_t offset, uint8_t value)
{
    if (_allocatedSize <= offset)
    {
        AllocateSize(offset + 1);
    }
    if (_size <= offset)
    {
        _size = offset + 1;
    }
    _data[offset] = value;
}

void ByteArray::SetUInt16(size_t offset, uint16_t value)
{
    SetUInt8(offset + 0, static_cast<uint8_t>((value >> 0) & 0xFF));
    SetUInt8(offset + 1, static_cast<uint8_t>((value >> 8) & 0xFF));
}

void ByteArray::SetUInt32(size_t offset, uint32_t value)
{
    SetUInt16(offset + 0, static_cast<uint16_t>((value >> 0) & 0xFFFF));
    SetUInt16(offset + 2, static_cast<uint16_t>((value >> 16) & 0xFFFF));
}

void ByteArray::SetUInt64(size_t offset, uint64_t value)
{
    SetUInt32(offset + 0, static_cast<uint32_t>((value >> 0) & 0xFFFFFFFF));
    SetUInt32(offset + 4, static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF));
}

void ByteArray::SetUInt16BE(size_t offset, uint16_t value)
{
    SetUInt8(offset + 0, static_cast<uint8_t>((value >> 8) & 0xFF));
    SetUInt8(offset + 1, static_cast<uint8_t>((value >> 0) & 0xFF));
}

void ByteArray::SetUInt32BE(size_t offset, uint32_t value)
{
    SetUInt16BE(offset + 0, static_cast<uint16_t>((value >> 16) & 0xFFFF));
    SetUInt16BE(offset + 2, static_cast<uint16_t>((value >> 0) & 0xFFFF));
}

void ByteArray::SetUInt64BE(size_t offset, uint64_t value)
{
    SetUInt32BE(offset + 0, static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF));
    SetUInt32BE(offset + 4, static_cast<uint32_t>((value >> 0) & 0xFFFFFFFF));
}

std::ostream & ByteArray::PrintTo(std::ostream & stream) const
{
    stream << OSAL::System::TypeName(*this) << " Size: " << _size << " Allocated: " << _allocatedSize << std::endl;
    size_t maxValuesToDisplay = std::min<size_t>(_size, MaxBytesToDisplay);
    for (size_t offset = 0; offset < maxValuesToDisplay; offset += BytesPerRow)
    {
        for (size_t i = 0; i < BytesPerRow; i++)
        {
            if (i + offset < maxValuesToDisplay)
            {
                uint8_t value = _data[i + offset];
                stream << std::hex << std::setw(2) << std::setfill('0') << (int)value << " ";
            }
            else
            {
                stream << "   ";
            }
        }
        for (size_t i = 0; i < BytesPerRow; i++)
        {
            if (i + offset < maxValuesToDisplay)
            {
                uint8_t value = _data[i + offset];
                stream << static_cast<char>(((value >= 32) && (value < 128)) ? value : '?') << " ";
            }
        }
        stream << std::endl;
    }
    stream << std::endl << std::flush;
    return stream;
}

} // namespace OSAL
