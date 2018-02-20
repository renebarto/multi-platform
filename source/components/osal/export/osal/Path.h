#pragma once

#include <osal/exports.h>

#include <string>

#if defined(WIN_MSVC)
#include "osal/windows/Path.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Path.h"
#elif defined(DARWIN)
#include "osal/darwin/Path.h"
#elif defined(LINUX)
#include "osal/linux/Path.h"
#endif

namespace OSAL {
namespace Path {

OSAL_EXPORT char PathSeparator();

OSAL_EXPORT std::string AddSeparatorIfNeeded(const std::string & path);
OSAL_EXPORT bool FileExists(const std::string & path);
OSAL_EXPORT bool DirectoryExists(const std::string & path);
OSAL_EXPORT void MakeSureDirectoryExists(const std::string & path);
OSAL_EXPORT void MakeSureFileDoesNotExist(const std::string & path);
OSAL_EXPORT std::string ResolveTilde(const std::string & path);
OSAL_EXPORT void SplitPath(const std::string & path, std::string & directory, std::string & fileName);
OSAL_EXPORT std::string Extension(const std::string & path);
OSAL_EXPORT std::string StripExtension(const std::string & path);
OSAL_EXPORT std::string LastPartOfPath(const std::string & path);
OSAL_EXPORT bool HasNoPath(const std::string & path);
OSAL_EXPORT std::string CreatePathWithDefault(const std::string & pathOrFileName, const std::string & defaultPath);
OSAL_EXPORT std::string CombinePath(const std::string & basePath, const std::string & subPath);
OSAL_EXPORT std::string FullPath(const std::string & path);
OSAL_EXPORT std::string RelativePath(const std::string & path);
OSAL_EXPORT std::string CurrentDir();
OSAL_EXPORT std::string StripPathToSubDirectory(const std::string & path, const std::string & subDirectoryName);
OSAL_EXPORT std::string HomePath();
OSAL_EXPORT std::string SystemBinariesPath();
OSAL_EXPORT std::string SystemLibrariesPath();
OSAL_EXPORT std::string TempPath();
OSAL_EXPORT std::string ResolveSymbolicLink(const std::string & path);
inline int ChangeCurrentDirectory(const std::string & directory)
{
    return ChDir(directory.c_str());
}
inline int CreateDirectory(const std::string & directory, mode_t mode)
{
    return MkDir(directory.c_str(), mode);
}
inline int RemoveDirectory(const std::string & directory)
{
    return RmDir(directory.c_str());
}

} // namespace Path
} // namespace OSAL
