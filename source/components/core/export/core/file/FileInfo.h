#pragma once

#include <memory>
#include <string>
#include <vector>
#include "FileSystemInfo.h"

namespace Core {
namespace File {

class DirectoryInfo;

class FileInfo
{
public:
    using Ptr = std::shared_ptr<FileInfo>;
    using List = std::vector<FileInfo>;

    FileInfo() = delete;
    FileInfo(const FileInfo &);
    FileInfo(FileInfo &&);

    FileInfo & operator = (const FileInfo &);
    FileInfo & operator = (FileInfo &&);

    size_t Length() const;
    void SetLength(size_t value);

    std::string const & GetPath() const;
    std::string GetDirectory() const;
    std::shared_ptr<DirectoryInfo> GetDirectoryInfo() const;
    std::string GetFilename() const;
    std::string GetFilenameWithoutExtension() const;
    std::string GetExtension() const;

protected:
    explicit FileInfo(const std::string & path);
};

} // namespace File
} // namespace Core
