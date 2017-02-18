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
    String name = typeid(x).name();
    const String classPrefix = _("class ");
    size_t pos = name.find(classPrefix, 0);
    if (pos == 0)
        name = name.substr(classPrefix.length());
    return name;
}

} // namespace OS
} // namespace OSAL