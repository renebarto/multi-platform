#include "re/TestData.h"

#include "osal/Path.h"

namespace RE {
namespace Test {

std::string Data::_applicationName = "";

std::string Data::ApplicationName()
{
    return OSAL::Path::ResolveSymbolicLink(_applicationName);
}

} // namespace Test
} // namespace RE
