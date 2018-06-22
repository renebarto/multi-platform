#include <core/files/BinaryStream.h>

#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

using namespace std;
using namespace Core;
using namespace Files;

BinaryStream::BinaryStream(std::iostream & other)
    : _stream(&other)
{
}

BinaryStream & BinaryStream::operator = (const BinaryStream & other)
{
    if (this != &other)
    {
        _stream = other._stream;

    }
    return *this;
}

BinaryStream & BinaryStream::operator = (BinaryStream && other)
{
    if (this != &other)
    {
        _stream = std::move(_stream);
    }
    return *this;
}

size_t BinaryStream::Read(uint8_t * buffer, size_t bufferSize)
{
    size_t bytesRead {};
    if (!Read(buffer, bufferSize, bytesRead))
        return 0;
    return bytesRead;
}

bool BinaryStream::Read(uint8_t * buffer, size_t bufferSize, size_t & bytesRead)
{
    bytesRead = static_cast<size_t>(_stream->readsome(reinterpret_cast<char *>(buffer), bufferSize));
    return _stream->good();
}

size_t BinaryStream::Write(const uint8_t * buffer, size_t bufferSize)
{
    size_t bytesWritten {};
    if (!Write(buffer, bufferSize, bytesWritten))
        return 0;
    return bytesWritten;
}

bool BinaryStream::Write(const uint8_t * buffer, size_t bufferSize, size_t & bytesWritten)
{
    _stream->write(reinterpret_cast<const char *>(buffer), bufferSize);
    if (_stream->good())
    {
        bytesWritten = bufferSize;
        return true;
    }
    return false;
}

bool BinaryStream::Read(char & ch)
{
    ch = {};
    _stream->read(&ch, sizeof(ch));
    return _stream->good();
}

bool BinaryStream::Read(int8_t & value)
{
    OSAL::ByteArray data(sizeof(int8_t));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value);
}

bool BinaryStream::Read(uint8_t & value)
{
    OSAL::ByteArray data(sizeof(uint8_t));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value);
}

bool BinaryStream::Read(int16_t & value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data(sizeof(int16_t));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value, endianness);
}

bool BinaryStream::Read(uint16_t & value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data(sizeof(uint16_t));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value, endianness);
}

bool BinaryStream::Read(int32_t & value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data(sizeof(int32_t));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value, endianness);
}

bool BinaryStream::Read(uint32_t & value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data(sizeof(uint32_t));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value, endianness);
}

bool BinaryStream::Read(int64_t & value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data(sizeof(int64_t));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value, endianness);
}

bool BinaryStream::Read(uint64_t & value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data(sizeof(uint64_t));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value, endianness);
}

bool BinaryStream::Read(float & value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data(sizeof(float));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value, endianness);
}

bool BinaryStream::Read(double & value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data(sizeof(double));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value, endianness);
}

bool BinaryStream::Read(long double & value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data(sizeof(long double));
    size_t bytesRead {};
    if (!Read(data.Data(), data.Size(), bytesRead) || bytesRead != data.Size())
        return false;
    return Deserialize(data, value, endianness);
}

bool BinaryStream::Read(std::string & value, size_t numChars)
{
    value = {};
    for (size_t i = 0; i < numChars; ++i)
    {
        char ch;
        if (!Read(ch))
            return false;
        value += ch;
    }
    return _stream->good();
}

bool BinaryStream::Read(OSAL::ByteArray & value, size_t numValues)
{
    value = {};
    OSAL::ByteArray result(numValues);
    _stream->read(reinterpret_cast<char *>(result.Data()), numValues);
    if (!_stream->good())
        return false;
    value = std::move(result);
    return true;
}

bool BinaryStream::Write(char ch)
{
    return _stream->write(&ch, sizeof(ch)).good();
}

bool BinaryStream::Write(int8_t value)
{
    OSAL::ByteArray data;
    Serialize(data, value);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(uint8_t value)
{
    OSAL::ByteArray data;
    Serialize(data, value);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(int16_t value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data;
    Serialize(data, value, endianness);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(uint16_t value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data;
    Serialize(data, value, endianness);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(int32_t value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data;
    Serialize(data, value, endianness);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(uint32_t value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data;
    Serialize(data, value, endianness);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(int64_t value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data;
    Serialize(data, value, endianness);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(uint64_t value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data;
    Serialize(data, value, endianness);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(float value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data;
    Serialize(data, value, endianness);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(double value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data;
    Serialize(data, value, endianness);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(long double value, OSAL::Endianness endianness)
{
    OSAL::ByteArray data;
    Serialize(data, value, endianness);
    data.Size(16);
    _stream->write(reinterpret_cast<const char *>(data.Data()), data.Size());
    return _stream->good();
}

bool BinaryStream::Write(const std::string & value)
{
    *_stream << value;
    return _stream->good();
}

bool BinaryStream::Write(const OSAL::ByteArray & value)
{
    _stream->write(reinterpret_cast<const char *>(value.Data()), value.Size());
    return _stream->good();
}

