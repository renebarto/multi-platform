#pragma once

#include <semaphore.h>
#include <unistd.h>
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

inline const char * getenv(const char * name)
{
    return ::getenv(name);
}

} // namespace System
} // namespace OSAL