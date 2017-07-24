#pragma once

#include "osal/Strings.h"

namespace OSAL {
namespace Path {

Char PathSeparator();
String LastPartOfPath(const String & path);

String AddSlashIfNeeded(const String & path);
bool FileExists(const String & path);
bool DirectoryExists(const String & path);
void MakeSureDirectoryExists(const String & path);
void MakeSureFileDoesNotExist(const String & path);
String ResolveTilde(const String & path);
void SplitPath(const String & path, String & directory, String & fileName);
String Extension(const String & path);
String StripExtension(const String & path);
String LastPartOfPath(const String & path);
bool HasNoPath(const String & path);
String CreatePathWithDefault(const String & pathOrFileName, const String & defaultPath);
String CombinePath(const String & basePath, const String & subPath);
String FullPath(const String & path);
String RelativePath(const String & path);
String CurrentDir();
String StripPathToSubDirectory(const String & path, const String & subDirectoryName);

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
