#include <core/files/BinaryFile.h>

#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

using namespace std;
using namespace Core;
using namespace Files;

BinaryFile::BinaryFile(const FileInfo & other)
    : File(other)
{
}

BinaryFile::BinaryFile(const FileInfo & other, DesiredAccess desiredAccess,
                   ShareMode shareMode,
                   CreationFlags creationFlags)
    : File(other)
{
    if ((desiredAccess & DesiredAccess::Binary) == 0)
        return;
    Open(desiredAccess | DesiredAccess::Binary, shareMode, creationFlags);
}

BinaryFile::BinaryFile(const std::string & path)
    : File(path)
{
}

BinaryFile::BinaryFile(const std::string & path, DesiredAccess desiredAccess,
                   ShareMode shareMode,
                   CreationFlags creationFlags)
    : File(path)
{
    if ((desiredAccess & DesiredAccess::Binary) == 0)
        return;
    Open(desiredAccess | DesiredAccess::Binary, shareMode, creationFlags);
}

BinaryFile::BinaryFile(const BinaryFile & other)
    : File(other)
{
    if ((other._openMode & (ios_base::in | ios_base::out)) != 0)
    {
        OpenInternal(other._openMode);
    }
}

BinaryFile::BinaryFile(BinaryFile && other)
    : File(std::move(other))
{
}

BinaryFile & BinaryFile::operator = (const BinaryFile & other)
{
    if (this != &other)
    {
        File::operator =(other);
    }
    return *this;
}

BinaryFile & BinaryFile::operator = (BinaryFile && other)
{
    if (this != &other)
    {
        File::operator =(std::move(other));
    }
    return *this;
}

bool BinaryFile::CompareTo(const std::string & pathOther)
{
    return Compare(FullPath(), pathOther);
}

bool BinaryFile::CompareTo(const BinaryFile & other)
{
    return CompareTo(other.FullPath());
}

bool BinaryFile::Compare(const std::string & source, const std::string & destination)
{
//    try
//    {
//        if (!Exists(source))
//            return false;
//        if (!Exists(destination))
//            return false;
//        BinaryFile sourceFile(source, DesiredAccess::ReadOnly);
//        BinaryFile destinationFile(destination, DesiredAccess::ReadOnly);
//
//        while (sourceFile.Good() && destinationFile.Good())
//        {
//            if (sourceFile.ReadLine() != destinationFile.ReadLine())
//                return false;
//        }
//    }
//    catch (...)
//    {
//        return false;
//    }
//    return true;
    return false;
}

bool BinaryFile::Read(char & ch)
{
    ch = {};
    _stream.read(&ch, sizeof(ch));
    return _stream.good();
}

bool BinaryFile::Read(int8_t & value)
{
//    value = {};
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//    return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(uint8_t & value)
{
//    value = {};
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//    return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(int16_t & value, OSAL::Endianness endianness)
{
//    value = {};
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//     return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(uint16_t & value, OSAL::Endianness endianness)
{
//    value = {};
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//    return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(int32_t & value, OSAL::Endianness endianness)
{
//    value = {};
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//    return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(uint32_t & value, OSAL::Endianness endianness)
{
//    value = {};
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//    return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(int64_t & value, OSAL::Endianness endianness)
{
//    value = {};
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//    return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(uint64_t & value, OSAL::Endianness endianness)
{
//    value = {};
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//    return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(float & value, OSAL::Endianness endianness)
{
//    value = {};
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//    return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(double & value, OSAL::Endianness endianness)
{
//    value = {};
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//    return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(long double & value, OSAL::Endianness endianness)
{
//    value = {};
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text;
//    _stream >> text;
//    if (!_stream.good())
//        return false;
//    return Deserialize(text, value);
    return false;
}

bool BinaryFile::Read(std::string & value, size_t numChars)
{
//    value = {};
//    for (size_t i = 0; i < numChars; ++i)
//    {
//        char ch;
//        if (!Read(ch))
//            return false;
//        value += ch;
//    }
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(char ch)
{
    return _stream.write(&ch, sizeof(ch)).good();
}

bool BinaryFile::Write(int8_t value)
{
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(uint8_t value)
{
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(int16_t value, OSAL::Endianness endianness)
{
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(uint16_t value, OSAL::Endianness endianness)
{
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(int32_t value, OSAL::Endianness endianness)
{
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(uint32_t value, OSAL::Endianness endianness)
{
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(int64_t value, OSAL::Endianness endianness)
{
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(uint64_t value, OSAL::Endianness endianness)
{
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(float value, OSAL::Endianness endianness)
{
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(double value, OSAL::Endianness endianness)
{
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(long double value, OSAL::Endianness endianness)
{
//    if (endianness != OSAL::Endianness::LittleEndian)
//        return false;
//    std::string text = Serialize(value);
//    _stream << text;
//    return _stream.good();
    return false;
}

bool BinaryFile::Write(const std::string & value)
{
//    _stream << value;
//    return _stream.good();
    return false;
}

