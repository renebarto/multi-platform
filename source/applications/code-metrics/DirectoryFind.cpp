#include "Core/DirectoryFind.h"

#include <algorithm>
#include "Core/Exception.h"
#include "Core/Util.h"

using namespace std;
using namespace Core;

bool DirectoryFind::stopScan = false;

DirectoryFind::DirectoryFind() :
    options(Options::Default),
    searchOption(SearchOption::Normal),
    skipHiddenDirs(false),
    skipRootDirs(false),
    directoryList(),
    searchResult()
{
}

DirectoryFind::DirectoryFind(Options options) :
    options(options),
    searchOption(((options & Options::Recursive) != 0) ? SearchOption::Recursive
                                                       : SearchOption::Normal),
    skipHiddenDirs((options & Options::SkipHiddenDirectories) != 0),
    skipRootDirs(((options & Options::OnlyLeafDirectories) != 0) &&
                 ((options & Options::Recursive) != 0)),
    directoryList(),
    searchResult()
{
}

DirectoryFind::~DirectoryFind()
{
}

size_t DirectoryFind::Search(const std::string & path, const std::string & wildcard, IProgressHandler * dialog)
{
    searchResult.clear();
    size_t count = 0;
    if (!DirectoryInfo::Exists(path))
    {
        return 0;
    }
    DirectoryInfo dirInfo = DirectoryInfo(path);
    stopScan = false;
    if (nullptr != dialog)
    {
        dialog->ProgressDialogShow();
        dialog->ProgressDialogInfo("Initializing");
        dialog->AddObserver(this);
    }
    directoryList = dirInfo.GetDirectories(wildcard, searchOption);
    std::sort(directoryList.begin(), directoryList.end(), CompareDirectories);
    int i = 0;
    int numDirectories = directoryList.size();
    if (nullptr != dialog)
    {
        dialog->ProgressDialogRange(numDirectories);
    }
    while (i < numDirectories && !stopScan)
    {
        if (nullptr != dialog)
        {
            dialog->ProgressDialogValue(i);
            dialog->ProgressDialogInfo(directoryList[i].FullPath());
        }
        bool isHidden = IsHiddenDirectory(directoryList[i]);
        bool hasSubDirectories = HasSubDirectories(directoryList[i], options);
        if (!(skipHiddenDirs && isHidden) && !(skipRootDirs && hasSubDirectories))
        {
            for (auto listener : *this)
            {
                stopScan = !listener->OnDirectoryFound(directoryList[i].FullPath());
            }
            searchResult.push_back(directoryList[i]);
            count++;
        }
        i++;
    }

    if (nullptr != dialog)
    {
        dialog->ProgressDialogValue(numDirectories);
        dialog->ProgressDialogInfo("Ready");
        dialog->ProgressDialogHide();
    }
    return count;
}

bool DirectoryFind::CompareDirectories(const DirectoryInfo & x, const DirectoryInfo & y)
{
    return x.FullPath() < y.FullPath();
}

void DirectoryFind::OnCancel()
{
    stopScan = true;
}

bool DirectoryFind::HasSubDirectories(const DirectoryInfo & directoryInfo, Options options)
{
    DirectoryFind subDirFinder(options);
    return (subDirFinder.Search(directoryInfo.FullPath(), "*") > 0);
}

bool DirectoryFind::IsHiddenDirectory(const DirectoryInfo & directoryInfo)
{
    if (directoryInfo.Name().empty())
        return false;
    return directoryInfo.Name()[0] == '.';
}
