#include "osal/Path.h"

#include <algorithm>

using namespace std;

namespace OSAL {
namespace Path {

static const char _Dot = '.';
static const char * _CurrentDir = ".";

static string ReplaceWithPathSeparator(const std::string & path)
{
    string result = path;
    if (PathSeparator() != '/')
        std::replace(result.begin(), result.end(), '/', PathSeparator());
    return result;
}

//void SplitPath(const string & path, string & directory, string & fileName)
//{
//    directory = {};
//    fileName = {};
//    string replacedPath = ReplaceWithPathSeparator(path);
//    if (DirectoryExists(replacedPath))
//    {
//        directory = replacedPath;
//        return;
//    }
//    size_t lastPathDelimiterPos = replacedPath.find_last_of(PathSeparator());
//    if (lastPathDelimiterPos != string::npos)
//    {
//        fileName = replacedPath.substr(lastPathDelimiterPos + 1);
//        directory = replacedPath.substr(0, lastPathDelimiterPos);
//        return;
//    }
//    fileName = replacedPath.substr(0, lastPathDelimiterPos);
//}
//
//string Extension(const string & path)
//{
//    size_t lastDotDelimiterPos = path.find_last_of(_Dot);
//    if ((lastDotDelimiterPos != 0) && (lastDotDelimiterPos != string::npos))
//    {
//        return path.substr(lastDotDelimiterPos);
//    }
//    return string();
//}
//
//string StripExtension(const string & path)
//{
//    size_t lastDotDelimiterPos = path.find_last_of(_Dot);
//    if ((lastDotDelimiterPos != 0) && (lastDotDelimiterPos != string::npos))
//    {
//        return path.substr(0, lastDotDelimiterPos);
//    }
//    return path;
//}
//
//string FirstPartOfPath(const string & path)
//{
//    string replacedPath = ReplaceWithPathSeparator(path);
//    size_t lastPathDelimiterPos = replacedPath.find_last_of(PathSeparator());
//    if (lastPathDelimiterPos != string::npos)
//    {
//        return replacedPath.substr(0, lastPathDelimiterPos);
//    }
//    return "";
//}
//
string LastPartOfPath(const string & path)
{
    string replacedPath = ReplaceWithPathSeparator(path);
    size_t lastPathDelimiterPos = replacedPath.find_last_of(PathSeparator());
    if (lastPathDelimiterPos != string::npos)
    {
        return replacedPath.substr(lastPathDelimiterPos + 1);
    }
    return replacedPath;
}

//bool HasNoPath(const string & path)
//{
//    string directory;
//    string fileName;
//    Path::SplitPath(path, directory, fileName);
//    return directory.empty();
//}
//
//string CreatePathWithDefault(const string & pathOrFileName, const string & defaultPath)
//{
//    if (HasNoPath(pathOrFileName))
//    {
//        return CombinePath(defaultPath, pathOrFileName);
//    }
//
//    return ReplaceWithPathSeparator(pathOrFileName);
//}
//
string CombinePath(const string & basePath, const string & subPath)
{
    string result = ReplaceWithPathSeparator(basePath);
    if (result.empty())
        result += _CurrentDir;
    if (result[result.length() - 1] != PathSeparator())
        result += PathSeparator();
    result += ReplaceWithPathSeparator(subPath);
    return result;
}

//string StripPathToSubDirectory(const string & path, const string & subDirectoryName)
//{
//    string strippedPath = ReplaceWithPathSeparator(path);
//    bool done = false;
//    while (!done)
//    {
//        size_t index = strippedPath.find_last_of(PathSeparator());
//        if (index != string::npos)
//        {
//            if (strippedPath.substr(index + 1) == subDirectoryName)
//            {
//                done = true;
//            }
//            else
//            {
//                strippedPath = strippedPath.substr(0, index);
//            }
//        }
//        else
//        {
//            return {};
//        }
//    }
//    return strippedPath;
//}
//
//string ResolveTilde(const string & path)
//{
//    string replacedPath = ReplaceWithPathSeparator(path);
//    if (replacedPath.length() < 2)
//        return replacedPath;
//    if (replacedPath.substr(0, 2) != string("~") + PathSeparator())
//        return replacedPath;
//    return CombinePath(HomePath(), replacedPath.substr(2));
//}

} // namespace Path
} // namespace OSAL
