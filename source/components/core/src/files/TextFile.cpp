#include <core/files/TextFile.h>

#include <fstream>
#include <osal/Path.h>
#include <osal/Unused.h>
#include <core/Util.h>
#include <limits>

using namespace std;
using namespace Core;
using namespace Files;

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
