#include <core/files/File.h>

#include <fstream>
#include <osal/Path.h>
#include <osal/Unused.h>
#include <core/Util.h>
#include <limits>

using namespace std;
using namespace Core;
using namespace Files;

#define BUFFER_SIZE 4096

File::File(const File & other)
    : FileInfo(other)
    , _stream()
    , _openMode()
{
    if ((other._openMode & (ios_base::in | ios_base::out)) != 0)
    {
        OpenInternal(other._openMode);
    }
}

File::File(File && other)
    : FileInfo(std::move(other))
    , _stream(std::move(other._stream))
    , _openMode(other._openMode)
{
    other.Close();
}

File::File(const std::string & path)
    : FileInfo(path)
    , _stream()
    , _openMode()
{
}

File::File(const std::string & path, DesiredAccess desiredAccess,
     ShareMode shareMode,
     CreationFlags creationFlags)
    : FileInfo(path)
    , _stream()
    , _openMode()
{
    Open(desiredAccess, shareMode, creationFlags);
}

File::File(const FileInfo & other)
    : FileInfo(other)
    , _stream()
    , _openMode()
{
}

File::File(const FileInfo & other, DesiredAccess desiredAccess,
           ShareMode shareMode,
           CreationFlags creationFlags)
    : FileInfo(other)
    , _stream()
    , _openMode()
{
    Open(desiredAccess, shareMode, creationFlags);
}

File & File::operator = (const File & other)
{
    if (this != &other)
    {
        FileInfo::operator =(other);
        Close();
        if ((other._openMode & (ios_base::in | ios_base::out)) != 0)
        {
            OpenInternal(other._openMode);
        }
    }
    return *this;
}

File & File::operator = (File && other)
{
    if (this != &other)
    {
        FileInfo::operator =(std::move(other));
        _stream = std::move(other._stream);
        _openMode = std::move(other._openMode);
        other.Close();
    }
    return *this;
}

std::iostream & File::GetStream()
{
    return _stream;
}

bool File::Good() const
{
    return _stream.good();
}

bool File::Eof() const
{
    return _stream.eof();
}

bool File::Open(DesiredAccess desiredAccess,
                ShareMode UNUSED(shareMode),
                CreationFlags creationFlags)
{
    Close();
    if ((desiredAccess & (DesiredAccess::WriteOnly | DesiredAccess::ReadOnly)) == DesiredAccess::ReadOnly)
    {
        if (!Exists() || ((creationFlags != CreationFlags::OpenExisting)))
            return false;
        ios_base::openmode openMode = ios_base::in;
        if ((desiredAccess & DesiredAccess::Binary) != 0)
            openMode |= ios_base::binary;

        return OpenInternal(openMode);
    }
    else
    {
        if ((desiredAccess & (DesiredAccess::WriteOnly | DesiredAccess::ReadOnly)) == 0)
            return false;
        ios_base::openmode openMode = ios_base::in | ios_base::out;
        if ((desiredAccess & DesiredAccess::Binary) != 0)
            openMode |= ios_base::binary;
        switch(creationFlags)
        {
            case CreationFlags::CreateNew:
                if (Exists())
                    return false;
                // Create file first
                if (!OpenInternal(ios_base::out | ios_base::binary))
                    return false;
                Close();
                break;
            case CreationFlags::CreateOrOverwrite:
                openMode |= ios_base::trunc;
                break;
            case CreationFlags::OpenExisting:
                if (!Exists())
                    return false;
                break;
            case CreationFlags::OpenOrCreate:
                if (!Exists())
                    openMode |= ios_base::trunc;
                break;
            case CreationFlags::TruncateExisting:
                if (!Exists())
                    return false;
                openMode |= ios_base::trunc;
                break;
        }
        return OpenInternal(openMode);
    }
}

void File::Close()
{
    if (_stream.is_open())
    {
        _stream.close();
    }
    _openMode = {};
}

bool File::IsOpen() const
{
    return _stream.is_open();
}

std::ios_base::openmode File::GetOpenMode() const
{
    return _openMode;
}

bool File::Delete()
{
    return Delete(FullPath());
}

bool File::CopyTo(const std::string & destination)
{
    return Copy(FullPath(), destination);
}

bool File::MoveTo(const std::string & destination)
{
    return Move(FullPath(), destination);
}

bool File::CopyTo(const File & file)
{
    return Copy(FullPath(), file.FullPath());
}

bool File::MoveTo(const File & file)
{
    return Move(FullPath(), file.FullPath());
}

bool File::Delete(const std::string & path)
{
    try
    {
        if (!Exists(path))
            return false;
        OSAL::Path::MakeSureFileDoesNotExist(path);
    }
    catch (std::exception & e)
    {
        return false;
    }
    return true;
}

bool File::Copy(const std::string & source, const std::string & destination)
{
    try
    {
        if (!Exists(source))
            return false;
        std::ifstream streamIn(source, std::ios::binary);
        std::ofstream streamOut(destination, std::ios::binary);

        uint8_t buffer[BUFFER_SIZE];
        std::streamsize bytesRead { std::numeric_limits<std::streamsize>::max()};
        while (streamIn.good() && streamOut.good() && (bytesRead != 0))
        {
            bytesRead = streamIn.readsome(reinterpret_cast<char *>(buffer), sizeof(buffer));
            streamOut.write(reinterpret_cast<char *>(buffer), bytesRead);
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool File::Move(const std::string & source, const std::string & destination)
{
    if (rename(source.c_str(), destination.c_str()) != 0)
        return false;
    return true;
}

bool File::CompareTo(const std::string & pathOther)
{
    return Compare(FullPath(), pathOther);
}

bool File::CompareTo(const File & other)
{
    return CompareTo(other.FullPath());
}

bool File::Compare(const std::string & source, const std::string & destination)
{
    try
    {
        if (!Exists(source))
            return false;
        if (!Exists(destination))
            return false;
        std::ifstream streamSource(source, std::ios::binary);
        std::ifstream streamDestination(destination, std::ios::binary);

        FileInfo fileSourceInfo(source);
        FileInfo fileDestinationInfo(destination);

        ssize_t fileSizeSource = fileSourceInfo.Size();
        ssize_t fileSizeDestination = fileDestinationInfo.Size();
        if ((fileSizeSource != fileSizeDestination))
            return false;
        uint8_t bufferSource[BUFFER_SIZE];
        uint8_t bufferDestination[BUFFER_SIZE];
        std::streamsize bytesReadSource { std::numeric_limits<std::streamsize>::max()};
        std::streamsize bytesReadDestination { std::numeric_limits<std::streamsize>::max()};
        while (streamSource.good() && streamDestination.good() && (bytesReadSource != 0) && (bytesReadDestination != 0))
        {
            bytesReadSource = streamSource.readsome(reinterpret_cast<char *>(bufferSource), sizeof(bufferSource));
            bytesReadDestination = streamDestination.readsome(reinterpret_cast<char *>(bufferDestination), sizeof(bufferDestination));
            if (bytesReadSource != bytesReadDestination)
                return false;
            if (!Util::Compare(bufferSource, bufferDestination, static_cast<int>(bytesReadSource)))
                return false;
        }
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool File::OpenInternal(std::ios_base::openmode openMode)
{
    _stream.open(FullPath().c_str(), openMode);
    _openMode = openMode;
    Stat();
    return _stream.good();
}