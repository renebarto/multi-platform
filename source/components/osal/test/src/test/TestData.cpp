#include "osal/TestData.h"

#include "osal/Exception.h"
#include "osal/Path.h"

namespace OSAL {
namespace Test {

std::string Data::_applicationName = "";

std::string Data::ApplicationName()
{
    return Path::ResolveSymbolicLink(_applicationName);
}

} // namespace Test
} // namespace OSAL
