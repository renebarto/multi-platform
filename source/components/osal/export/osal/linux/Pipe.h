#pragma once

#include <unistd.h>

namespace OSAL {
namespace Pipe {

inline int pipe(int fds[2])
{
    return ::pipe(fds);
}

} // namespace Pipe
} // namespace OSAL