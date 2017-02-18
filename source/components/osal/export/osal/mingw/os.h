#pragma once

#include <typeinfo>
#include "osal/strings.h"

namespace OSAL {
namespace OS {

OSAL_EXPORT String Name();
OSAL_EXPORT String Variant();
OSAL_EXPORT String Release();
OSAL_EXPORT String Version();
OSAL_EXPORT String Platform();

OSAL_EXPORT String DemangleName(const String & mangledName);

template <class T>
inline String TypeName(const T & x)
{
    return ConvertTypeName(typeid(x).name());
}

} // namespace OS
} // namespace OSAL