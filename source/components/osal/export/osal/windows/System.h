#pragma once

#include <cstdlib>
#include <string>
#include <typeinfo>

namespace OSAL {
namespace System {

OSAL_EXPORT std::string Name();
OSAL_EXPORT std::string Variant();
OSAL_EXPORT std::string Release();
OSAL_EXPORT std::string Version();
OSAL_EXPORT std::string Platform();

OSAL_EXPORT std::string DemangleName(const std::string & mangledName);

template <class T>
inline std::string TypeName(const T & x)
{
    return DemangleName(typeid(x).name());
}

inline const char * GetEnvironmentVariable(const char * name)
{
    static char buffer[4096];
    if (::GetEnvironmentVariableA(name, buffer, sizeof(buffer)) == 0)
    {
        return nullptr;
    }
    return buffer;
}
inline int SetEnvironmentVariable(const char * name, const char * value, bool overwrite = true)
{
    static char buffer[4096];
    if (!overwrite && ::GetEnvironmentVariableA(name, buffer, sizeof(buffer)) != 0)
        return 0;
    return ::SetEnvironmentVariableA(name, value) ? 0 : EINVAL;
}
inline int UnSetEnvironmentVariable(const char * name)
{
    return ::SetEnvironmentVariableA(name, nullptr) ? 0 : EINVAL;
}

} // namespace System
} // namespace OSAL