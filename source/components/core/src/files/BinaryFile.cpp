#include <core/files/BinaryFile.h>

#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

using namespace std;
using namespace Core;
using namespace Files;

BinaryFile::BinaryFile(const FileInfo & other)
    : BinaryStream(GetStream())
    , File(other)
{
}

BinaryFile::BinaryFile(const FileInfo & other, DesiredAccess desiredAccess,
                   ShareMode shareMode,
                   CreationFlags creationFlags)
    : BinaryStream(GetStream())
    , File(other)
{
    if ((desiredAccess & DesiredAccess::Text) != 0)
        return;
    Open(desiredAccess | DesiredAccess::Binary, shareMode, creationFlags);
}

BinaryFile::BinaryFile(const std::string & path)
    : BinaryStream(GetStream())
    , File(path)
{
}

BinaryFile::BinaryFile(const std::string & path, DesiredAccess desiredAccess,
                   ShareMode shareMode,
                   CreationFlags creationFlags)
    : BinaryStream(GetStream())
    , File(path)
{
    if ((desiredAccess & DesiredAccess::Text) != 0)
        return;
    Open(desiredAccess | DesiredAccess::Binary, shareMode, creationFlags);
}

BinaryFile::BinaryFile(const BinaryFile & other)
    : BinaryStream(GetStream())
    , File(other)
{
    if ((other._openMode & (ios_base::in | ios_base::out)) != 0)
    {
        OpenInternal(other._openMode);
    }
}

BinaryFile::BinaryFile(BinaryFile && other)
    : BinaryStream(GetStream())
    , File(std::move(other))
{
}

BinaryFile & BinaryFile::operator = (const BinaryFile & other)
{
    if (this != &other)
    {
        File::operator =(other);
        BinaryStream::operator =(other);
    }
    return *this;
}

BinaryFile & BinaryFile::operator = (BinaryFile && other)
{
    if (this != &other)
    {
        File::operator =(std::move(other));
        BinaryStream::operator =(std::move(other));
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
    try
    {
        if (!Exists(source))
            return false;
        if (!Exists(destination))
            return false;
        BinaryFile sourceFile(source, DesiredAccess::ReadOnly);
        BinaryFile destinationFile(destination, DesiredAccess::ReadOnly);
        const size_t BufferSize = 4096;
        uint8_t sourceData[BufferSize];
        uint8_t destinationData[BufferSize];

        while (sourceFile.Good() && destinationFile.Good())
        {
            size_t sourceDataRead {};
            size_t destinationDataRead {};
            if (!sourceFile.Read(sourceData, sizeof(sourceData), sourceDataRead))
                return false;
            if (!destinationFile.Read(destinationData, sizeof(destinationData), destinationDataRead))
                return false;
            if (sourceDataRead != destinationDataRead)
                return false;
            if (!Util::Compare(sourceData, destinationData, sourceDataRead))
                return false;
            if (sourceDataRead == 0)
                break;
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

