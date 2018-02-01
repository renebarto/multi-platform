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
    return ::getenv(name);
}
inline int SetEnvironmentVariable(const char * name, const char * value, bool overwrite = true)
{
    return ::setenv(name, value, overwrite ? 1 : 0);
}
inline int UnSetEnvironmentVariable(const char * name)
{
    return ::unsetenv(name);
}

} // namespace System
} // namespace OSAL