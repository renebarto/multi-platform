#include <core/files/TextFile.h>

#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

using namespace std;
using namespace Core;
using namespace Files;

TextFile::TextFile(const FileInfo & other)
    : TextStream(GetStream())
    , File(other)
{
}

TextFile::TextFile(const FileInfo & other, DesiredAccess desiredAccess,
                   ShareMode shareMode,
                   CreationFlags creationFlags)
    : TextStream(GetStream())
    , File(other)
{
    if ((desiredAccess & DesiredAccess::Binary) != 0)
        return;
    Open(desiredAccess | DesiredAccess::Text, shareMode, creationFlags);
}

TextFile::TextFile(const std::string & path)
    : TextStream(GetStream())
    , File(path)
{
}

TextFile::TextFile(const std::string & path, DesiredAccess desiredAccess,
                   ShareMode shareMode,
                   CreationFlags creationFlags)
    : TextStream(GetStream())
    , File(path)
{
    if ((desiredAccess & DesiredAccess::Binary) != 0)
        return;
    Open(desiredAccess | DesiredAccess::Text, shareMode, creationFlags);
}

TextFile::TextFile(const TextFile & other)
    : TextStream(GetStream())
    , File(other)
{
    if ((other._openMode & (ios_base::in | ios_base::out)) != 0)
    {
        OpenInternal(other._openMode);
    }
}

TextFile::TextFile(TextFile && other)
    : TextStream(GetStream())
    , File(std::move(other))
{
}

TextFile & TextFile::operator = (const TextFile & other)
{
    if (this != &other)
    {
        File::operator =(other);
        TextStream::operator =(other);
    }
    return *this;
}

TextFile & TextFile::operator = (TextFile && other)
{
    if (this != &other)
    {
        File::operator =(std::move(other));
        TextStream::operator =(std::move(other));
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

