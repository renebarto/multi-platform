#pragma once

#include <string>
#include <memory>
#include "core/files/DirectoryInfo.h"
#include "core/files/FileSystemInfo.h"

namespace Core {
namespace Files {

class DirectoryInfo;

class FileInfo : public FileSystemInfo
{
public:
    typedef std::shared_ptr<FileInfo> Ptr;
    typedef std::vector<FileInfo> List;

    FileInfo() = delete;
    FileInfo(const std::string & path);
    FileInfo(const FileInfo &);
    FileInfo(FileInfo &&);
    FileInfo & operator = (const FileInfo &);
    FileInfo & operator = (FileInfo &&);

    ssize_t Size() const { return _fileSize; }
    bool Exists() const;
    bool IsHiddenFile() const;

    static bool Exists(const std::string & path);
    static bool IsHiddenFile(const std::string & name);

    std::string const & GetPath() const;
    std::string GetDirectoryName() const;
    std::string GetFilename() const;
    std::string GetFilenameWithoutExtension() const;
    std::string GetExtension() const;
    std::shared_ptr<DirectoryInfo> GetDirectory() const { return Parent(); }

protected:
    friend class DirectoryInfo;
};

} // namespace Files
} // namespace Core
