#include "osal/Path.h"

using namespace std;

namespace OSAL {
namespace Path {

static const char _Dot = '.';
static const char * _CurrentDir = ".";

void SplitPath(const string & path, string & directory, string & fileName)
{
    directory = {};
    fileName = {};
    if (DirectoryExists(path))
    {
        directory = path;
        return;
    }
    size_t lastPathDelimiterPos = path.find_last_of(PathSeparator());
    if (lastPathDelimiterPos != string::npos)
    {
        fileName = path.substr(lastPathDelimiterPos + 1);
        directory = path.substr(0, lastPathDelimiterPos);
        return;
    }
    fileName = path.substr(0, lastPathDelimiterPos);
}

string Extension(const string & path)
{
    size_t lastPathDelimiterPos = path.find_last_of(_Dot);
    if (lastPathDelimiterPos != string::npos)
    {
        return path.substr(lastPathDelimiterPos);
    }
    return string();
}

string StripExtension(const string & path)
{
    size_t lastPathDelimiterPos = path.find_last_of(_Dot);
    if (lastPathDelimiterPos != string::npos)
    {
        return path.substr(0, lastPathDelimiterPos);
    }
    return path;
}

string LastPartOfPath(const string & path)
{
    size_t lastPathDelimiterPos = path.find_last_of(PathSeparator());
    if (lastPathDelimiterPos != string::npos)
    {
        return path.substr(lastPathDelimiterPos + 1);
    }
    return path;
}

bool HasNoPath(const string & path)
{
    string directory;
    string fileName;
    Path::SplitPath(path, directory, fileName);
    return directory.empty();
}

string CreatePathWithDefault(const string & pathOrFileName, const string & defaultPath)
{
    if (HasNoPath(pathOrFileName))
    {
        return CombinePath(defaultPath, pathOrFileName);
    }
    return pathOrFileName;
}

string CombinePath(const string & basePath, const string & subPath)
{
    string result = basePath;
    if (result.empty())
        result += _CurrentDir;
    if (result[result.length() - 1] != PathSeparator())
        result += PathSeparator();
    result += subPath;
    return result;
}

string StripPathToSubDirectory(const string & path, const string & subDirectoryName)
{
    string strippedPath = path;
    bool done = false;
    while (!done)
    {
        size_t index = strippedPath.find_last_of(PathSeparator());
        if (index != string::npos)
        {
            if (strippedPath.substr(index + 1) == subDirectoryName)
            {
                done = true;
            }
            else
            {
                strippedPath = strippedPath.substr(0, index);
            }
        }
        else
        {
			return {};
        }
    }
    return strippedPath;
}

} // namespace Path
} // namespace OSAL
