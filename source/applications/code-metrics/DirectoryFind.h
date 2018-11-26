#pragma once

#include <string>
#include <vector>
//#include "Core/DirectoryInfo.h"
//#include "Core/IProgressHandler.h"

namespace Core
{

class IDirectoryFindCallback
{
public:
    virtual ~IDirectoryFindCallback() {}

    virtual bool OnDirectoryFound(const std::string & path) = 0;
};

class DirectoryFind : public Core::observable<IDirectoryFindCallback>, public ICancellable
{
public:

    enum class Options : int
    {
        Default = 0x00,
        Recursive = 0x01,
        OnlyLeafDirectories = 0x02,
        SkipHiddenDirectories = 0x04,
    };

    DirectoryFind();
    DirectoryFind(const DirectoryFind &) = delete;
    explicit DirectoryFind(Options options);
    virtual ~DirectoryFind();

    DirectoryFind & operator = (const DirectoryFind &) = delete;

    const std::vector<DirectoryInfo> & SearchResult() { return searchResult; }

    size_t Search(const std::string & path, const std::string & wildcard, IProgressHandler * dialog = nullptr);

protected:
    static bool CompareDirectories(const DirectoryInfo & x, const DirectoryInfo & y);
    static bool IsHiddenDirectory(const DirectoryInfo & directoryInfo);

    void OnCancel();

    static bool HasSubDirectories(const DirectoryInfo & directoryInfo, Options options);


private:
    friend class FileFind;

    Options options;
    SearchOption searchOption;
    bool skipHiddenDirs;
    bool skipRootDirs;

    static bool stopScan;

    DirectoryInfo::List directoryList;
    DirectoryInfo::List searchResult;
};

DEFINE_FLAG_OPERATORS(DirectoryFind::Options, int);

} // namespace Core
