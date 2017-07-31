#pragma once

#include <typeinfo>
#include "osal/Strings.h"

namespace OSAL {
namespace OS {

OSAL_EXPORT String Name();
OSAL_EXPORT String Variant();
OSAL_EXPORT String Release();
OSAL_EXPORT String Version();
OSAL_EXPORT String Platform();

OSAL_EXPORT String DemangleName(const String & mangledName);

WARNING_PUSH
WARNING_DISABLE(4100)
template <class T>
inline String TypeName(const T & x)
{
    String name = ToString(typeid(x).name());
    const String classPrefix = _("class ");
    size_t pos = name.find(classPrefix, 0);
    if (pos == 0)
        name = name.substr(classPrefix.length());
    return name;
}
WARNING_POP

} // namespace OS
} // namespace OSAL