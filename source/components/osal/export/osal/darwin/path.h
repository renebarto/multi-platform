#pragma once

#include "osal/osal.h"

namespace OSAL {
namespace Path {

static const Char _PathSeparator = _('/');

inline Char PathSeparator()
{
    return _PathSeparator;
}

inline String LastPartOfPath(const String & path)
{
    size_t lastPathDelimiterPos = path.find_last_of(PathSeparator());
    if (lastPathDelimiterPos != String::npos)
    {
        return path.substr(lastPathDelimiterPos + 1);
    }
    return path;
}

} // namespace Path
} // namespace OSAL