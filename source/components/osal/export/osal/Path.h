#pragma once

#include "osal/Strings.h"

namespace OSAL {
namespace Path {

OSAL_EXPORT Char PathSeparator();
OSAL_EXPORT String LastPartOfPath(const String & path);

OSAL_EXPORT String AddSlashIfNeeded(const String & path);
OSAL_EXPORT bool FileExists(const String & path);
OSAL_EXPORT bool DirectoryExists(const String & path);
OSAL_EXPORT void MakeSureDirectoryExists(const String & path);
OSAL_EXPORT void MakeSureFileDoesNotExist(const String & path);
OSAL_EXPORT String ResolveTilde(const String & path);
OSAL_EXPORT void SplitPath(const String & path, String & directory, String & fileName);
OSAL_EXPORT String Extension(const String & path);
OSAL_EXPORT String StripExtension(const String & path);
OSAL_EXPORT String LastPartOfPath(const String & path);
OSAL_EXPORT bool HasNoPath(const String & path);
OSAL_EXPORT String CreatePathWithDefault(const String & pathOrFileName, const String & defaultPath);
OSAL_EXPORT String CombinePath(const String & basePath, const String & subPath);
OSAL_EXPORT String FullPath(const String & path);
OSAL_EXPORT String RelativePath(const String & path);
OSAL_EXPORT String CurrentDir();
OSAL_EXPORT String StripPathToSubDirectory(const String & path, const String & subDirectoryName);

} // namespace Path
} // namespace OSAL

#if defined(WIN_MSVC)
#include "osal/windows/Path.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Path.h"
#elif defined(DARWIN)
#include "osal/darwin/Path.h"
#elif defined(LINUX)
#include "osal/linux/Path.h"
#endif
