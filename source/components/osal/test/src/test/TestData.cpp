#include "osal/TestData.h"

#include "osal/Exception.h"

namespace OSAL {
namespace Test {

std::string Data::_applicationName = "";

std::string ResolvedSymbolicLink(const std::string & path)
{
    std::string result = path;
    bool done = false;
    while (!done)
    {
        constexpr size_t BufferSize = 4096;
        char buffer[BufferSize];
        ssize_t returnValue = readlink(result.c_str(), buffer, BufferSize);
        if (returnValue < 0)
        {
            if (errno == EINVAL)
            {
                done = true;
                continue;
            }
            ThrowOnError(__func__, __FILE__, __LINE__, errno);
        }
        buffer[returnValue] = '\0';
        result = buffer;
    }
    return result;
}

std::string Data::ApplicationName()
{
    return ResolvedSymbolicLink(_applicationName);
}

} // namespace Test
} // namespace OSAL
