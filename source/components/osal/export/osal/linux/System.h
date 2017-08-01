#pragma once

#include <cstdlib>

namespace OSAL {
namespace System {

inline const char * getenv(const char * name)
{
    return ::getenv(name);
}
inline const wchar_t * getenv(const wchar_t * name)
{
    std::string nameNarrow = OSAL::WStringToString(name);
    static std::wstring buffer;
    const char * result = ::getenv(nameNarrow.c_str());
    if (result == nullptr)
    {
        return nullptr;
    }
    buffer = ToWideString(result);
    return buffer.c_str();
}

inline int chdir(const char * path)
{
    return ::chdir(path);
}
inline int chdir(const wchar_t * path)
{
    std::string pathNarrow = OSAL::WStringToString(path);
    return ::chdir(pathNarrow.c_str());
}

} // namespace System
} // namespace OSAL