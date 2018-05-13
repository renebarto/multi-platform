#include <core/files/File.h>

#include <fstream>
#include "osal/Path.h"

using namespace std;
using namespace Core;
using namespace Files;

#define BUFFER_SIZE 4096

//File::File(const File &)
//{
//
//}
//
//File::File(File &&)
//{
//
//}
//
//File::File(const std::string & path)
//{
//
//}
//
//File::File(const std::string & path, DesiredAccess desiredAccess,
//     ShareMode shareMode,
//     CreationFlags creationFlags)
//{
//
//}
//
//File & File::operator = (const File &)
//{
//
//}
//
//File & File::operator = (File &&)
//{
//
//}
//
//std::iostream * File::GetStream()
//{
//
//}
//
//bool File::Open(DesiredAccess desiredAccess,
//          ShareMode shareMode,
//          CreationFlags creationFlags)
//{
//
//}
//
//void File::Close()
//{
//
//}
//
//bool File::IsOpen() const
//{
//
//}
//
//bool File::Exists() const
//{
//
//}
//
//bool File::Exists(const std::string & path)
//{
//
//}
//
void File::Delete()
{
    Delete(FullPath());
}

void File::CopyTo(const std::string & destination)
{
    Copy(FullPath(), destination);
}

void File::MoveTo(const std::string & destination)
{
    Move(FullPath(), destination);
}

//bool File::Copy(File & file)
//{
//
//}
//
//bool File::Move(File & file)
//{
//
//}
//
void File::Delete(const std::string & path)
{
    OSAL::Path::MakeSureFileDoesNotExist(path);
}

bool File::Copy(const std::string & source, const std::string & destination)
{
    try
    {
        std::ifstream streamIn(source, std::ios::binary);
        std::ofstream streamOut(destination, std::ios::binary);

        uint8_t buffer[BUFFER_SIZE];
        while (streamIn && streamOut)
        {
            std::streamsize bytesRead = streamIn.readsome(reinterpret_cast<char *>(buffer), sizeof(buffer));
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

//bool File::IsHiddenFile() const
//{
//
//}
//
//bool File::IsHiddenFile(const std::string & name)
//{
//
//}
//
