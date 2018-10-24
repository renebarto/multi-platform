#pragma once

#include <string>
#include <memory>
#include "DirectoryInfo.h"
#include "FileSystemInfo.h"
#include "Core/Timestamp.h"

namespace Core
{

class DirectoryInfo;


class FileInfo : public FileSystemInfo
{
public:
    typedef std::shared_ptr<FileInfo> Ptr;
    typedef std::vector<FileInfo> List;

    FileInfo() = delete;

    long Length() const { return length; }
    std::string DirectoryName() const;
    std::shared_ptr<DirectoryInfo> Directory() const;
    bool Exists() const;
    static bool Exists(const std::string & path);
    void Delete();
    static void Delete(const std::string & path);
    void CopyTo(const std::string & destination);
    void Copy(const std::string & source, const std::string & destination);
    void MoveTo(const std::string & destination);
    void Move(const std::string & source, const std::string & destination);
    bool IsHiddenFile() const;
    bool IsHiddenFile(const std::string & name) const;

protected:
    explicit FileInfo(const std::string & path);

    friend class DirectoryInfo;
};


} // namespace Core
