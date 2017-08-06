#pragma once

#include <cstdlib>
#include <direct.h>

namespace OSAL {
namespace System {

inline const char * getenv(const char * name)
{
    static char buffer[4096];
    if (::GetEnvironmentVariableA(name, buffer, sizeof(buffer)) == 0)
    {
        return nullptr;
    }
    return buffer;
}
inline const wchar_t * getenv(const wchar_t * name)
{
    static wchar_t buffer[4096];
    if (::GetEnvironmentVariableW(name, buffer, sizeof(buffer)) == 0)
    {
        return nullptr;
    }
    return buffer;
}

inline int chdir(const char * path)
{
    return ::_chdir(path);
}
inline int chdir(const wchar_t * path)
{
    return ::_wchdir(path);
}


} // namespace System
} // namespace OSAL