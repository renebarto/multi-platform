#pragma once

#include <cstdlib>

namespace OSAL {
namespace System {

inline const char * GetEnv(const char * name)
{
    return ::getenv(name);
}

} // namespace System
} // namespace OSAL