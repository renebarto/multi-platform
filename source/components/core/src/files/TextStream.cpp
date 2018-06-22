#include <core/files/TextStream.h>

#include <osal/Strings.h>
#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

using namespace std;
using namespace Core;
using namespace Files;

TextStream::TextStream(std::iostream & other)
    : _stream(&other)
{
}

TextStream & TextStream::operator = (const TextStream & other)
{
    if (this != &other)
    {
        _stream = other._stream;

    }
    return *this;
}

TextStream & TextStream::operator = (TextStream && other)
{
    if (this != &other)
    {
        _stream = std::move(_stream);
    }
    return *this;
}

char TextStream::ReadChar()
{
    char ch;
    if (!Read(ch))
        return '\0';
    return ch;
}

std::string TextStream::ReadLine()
{
    std::string result;
    if (!ReadLine(result))
        return {};
    return result;
}

bool TextStream::ReadLine(std::string & line)
{
    return getline(*_stream, line).good();
}

bool TextStream::WriteLine(const std::string & line)
{
    for (auto ch : line)
        if (!Write(ch))
            return false;
    return Write('\n');
}

bool TextStream::ReadString(std::string & value, const std::string & delimiter)
{
    value = {};
    char ch;
    while (Read(ch))
    {
        if (delimiter.find(ch) != string::npos)
        {
            break;
        }
        value += ch;
    }
    return true;
}

bool TextStream::ReadAll(std::string & value)
{
    value = {};
    char ch;
    while (Read(ch))
    {
        value += ch;
    }
    return _stream->eof();
}

bool TextStream::Read(char & ch)
{
    ch = {};
    return _stream->get(ch).good();
}

bool TextStream::Read(int8_t & value)
{
    value = {};
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
    return Deserialize(text, value);
}

bool TextStream::Read(uint8_t & value)
{
    value = {};
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
    return Deserialize(text, value);
}

bool TextStream::Read(int16_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
     return Deserialize(text, value);
}

bool TextStream::Read(uint16_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
    return Deserialize(text, value);
}

bool TextStream::Read(int32_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
    return Deserialize(text, value);
}

bool TextStream::Read(uint32_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
    return Deserialize(text, value);
}

bool TextStream::Read(int64_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
    return Deserialize(text, value);
}

bool TextStream::Read(uint64_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
    return Deserialize(text, value);
}

bool TextStream::Read(float & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
    return Deserialize(text, value);
}

bool TextStream::Read(double & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
    return Deserialize(text, value);
}

bool TextStream::Read(long double & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    *_stream >> text;
    if (!_stream->good())
        return false;
    return Deserialize(text, value);
}

bool TextStream::Read(std::string & value, size_t numChars)
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

bool TextStream::Read(OSAL::ByteArray & value, size_t numValues)
{
    value = {};
    OSAL::ByteArray result(numValues);
    string str;
    size_t index {};
    while (index < numValues)
    {
        char ch;
        if (!Read(ch))
            return false;
        if (OSAL::Strings::isspace(ch))
        {
            uint8_t elementValue {};
            if (!Deserialize(str, elementValue, 16))
                return false;
            result.SetUInt8(index, elementValue);
            ++index;
            str = {};
        }
        else
            str += ch;
    }
    value = std::move(result);
    return _stream->good();
}

bool TextStream::Write(char ch)
{
    return _stream->put(ch).good();
}

bool TextStream::Write(int8_t value)
{
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(uint8_t value)
{
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(int16_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(uint16_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(int32_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(uint32_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(int64_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(uint64_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(float value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(double value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(long double value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    *_stream << text;
    return _stream->good();
}

bool TextStream::Write(const std::string & value)
{
    *_stream << value;
    return _stream->good();
}

bool TextStream::Write(const OSAL::ByteArray & value)
{
    for (size_t i = 0; i < value.Size(); ++i)
    {
        Write(Serialize(value[i], 16));
        if (i < value.Size() - 1)
            Write(' ');
    }
    return _stream->good();
}

