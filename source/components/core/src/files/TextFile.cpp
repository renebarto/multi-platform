#include <core/files/TextFile.h>

#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

using namespace std;
using namespace Core;
using namespace Files;

TextFile::TextFile(const FileInfo & other)
    : File(other)
{
}

TextFile::TextFile(const FileInfo & other, DesiredAccess desiredAccess,
                   ShareMode shareMode,
                   CreationFlags creationFlags)
    : File(other)
{
    if ((desiredAccess & DesiredAccess::Binary) != 0)
        return;
    Open(desiredAccess | DesiredAccess::Text, shareMode, creationFlags);
}

TextFile::TextFile(const std::string & path)
    : File(path)
{
}

TextFile::TextFile(const std::string & path, DesiredAccess desiredAccess,
                   ShareMode shareMode,
                   CreationFlags creationFlags)
    : File(path)
{
    if ((desiredAccess & DesiredAccess::Binary) != 0)
        return;
    Open(desiredAccess | DesiredAccess::Text, shareMode, creationFlags);
}

TextFile::TextFile(const TextFile & other)
    : File(other)
{
    if ((other._openMode & (ios_base::in | ios_base::out)) != 0)
    {
        OpenInternal(other._openMode);
    }
}

TextFile::TextFile(TextFile && other)
    : File(std::move(other))
{
}

TextFile & TextFile::operator = (const TextFile & other)
{
    if (this != &other)
    {
        File::operator =(other);
    }
    return *this;
}

TextFile & TextFile::operator = (TextFile && other)
{
    if (this != &other)
    {
        File::operator =(std::move(other));
    }
    return *this;
}

bool TextFile::CompareTo(const std::string & pathOther)
{
    return Compare(FullPath(), pathOther);
}

bool TextFile::CompareTo(const TextFile & other)
{
    return CompareTo(other.FullPath());
}

bool TextFile::Compare(const std::string & source, const std::string & destination)
{
    try
    {
        if (!Exists(source))
            return false;
        if (!Exists(destination))
            return false;
        TextFile sourceFile(source, DesiredAccess::ReadOnly);
        TextFile destinationFile(destination, DesiredAccess::ReadOnly);

        while (sourceFile.Good() && destinationFile.Good())
        {
            if (sourceFile.ReadLine() != destinationFile.ReadLine())
                return false;
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

char TextFile::ReadChar()
{
    char ch;
    if (!Read(ch))
        return '\0';
    return ch;
}

std::string TextFile::ReadLine()
{
    std::string result;
    if (!ReadLine(result))
        return {};
    return result;
}

bool TextFile::ReadLine(std::string & line)
{
    return getline(_stream, line).good();
}

bool TextFile::WriteLine(const std::string & line)
{
    for (auto ch : line)
        if (!Write(ch))
            return false;
    return Write('\n');
}

bool TextFile::ReadString(std::string & value, const std::string & delimiter)
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

bool TextFile::ReadAll(std::string & value)
{
    value = {};
    char ch;
    while (Read(ch))
    {
        value += ch;
    }
    return _stream.eof();
}

bool TextFile::Read(char & ch)
{
    ch = {};
    return _stream.get(ch).good();
}

bool TextFile::Read(int8_t & value)
{
    value = {};
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
    return Deserialize(text, value);
}

bool TextFile::Read(uint8_t & value)
{
    value = {};
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
    return Deserialize(text, value);
}

bool TextFile::Read(int16_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
     return Deserialize(text, value);
}

bool TextFile::Read(uint16_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
    return Deserialize(text, value);
}

bool TextFile::Read(int32_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
    return Deserialize(text, value);
}

bool TextFile::Read(uint32_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
    return Deserialize(text, value);
}

bool TextFile::Read(int64_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
    return Deserialize(text, value);
}

bool TextFile::Read(uint64_t & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
    return Deserialize(text, value);
}

bool TextFile::Read(float & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
    return Deserialize(text, value);
}

bool TextFile::Read(double & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
    return Deserialize(text, value);
}

bool TextFile::Read(long double & value, OSAL::Endianness endianness)
{
    value = {};
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text;
    _stream >> text;
    if (!_stream.good())
        return false;
    return Deserialize(text, value);
}

bool TextFile::Read(std::string & value, size_t numChars)
{
    value = {};
    for (size_t i = 0; i < numChars; ++i)
    {
        char ch;
        if (!Read(ch))
            return false;
        value += ch;
    }
    return _stream.good();
}

bool TextFile::Write(char ch)
{
    return _stream.put(ch).good();
}

bool TextFile::Write(int8_t value)
{
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(uint8_t value)
{
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(int16_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(uint16_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(int32_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(uint32_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(int64_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(uint64_t value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(float value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(double value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(long double value, OSAL::Endianness endianness)
{
    if (endianness != OSAL::Endianness::LittleEndian)
        return false;
    std::string text = Serialize(value);
    _stream << text;
    return _stream.good();
}

bool TextFile::Write(const std::string & value)
{
    _stream << value;
    return _stream.good();
}

