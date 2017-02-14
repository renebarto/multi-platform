#pragma once

#include "osal/strings.h"

namespace OSAL {
namespace OS {

String Name();
String Variant();
String Release();
String Version();
String Platform();

String DemangleName(const String & mangledName);

} // namespace OS
} // namespace OSAL