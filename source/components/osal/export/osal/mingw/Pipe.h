#pragma once

#include <fcntl.h>
#include <io.h>

namespace OSAL {
namespace Pipe {

inline int pipe(int fds[2])
{
    return ::_pipe(fds, 4096, _O_BINARY);
}

} // namespace Pipe
} // namespace OSAL