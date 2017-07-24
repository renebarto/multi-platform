#pragma once

#include <iostream>
#include <memory>
#include <osal/Strings.h>

namespace OSAL {
namespace OS {

class TemporaryFile
{
public:
    TemporaryFile();
    ~TemporaryFile();
    void Close();
    std::iostream & GetStream();
    String const & GetPath() const;

private:
    std::shared_ptr<std::iostream> _stream;
    String _path;
};

} // namespace OS
} // namespace OSAL

#if defined(WIN_MSVC)
#include "osal/windows/OS.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/OS.h"
#elif defined(DARWIN)
#include "osal/darwin/OS.h"
#elif defined(LINUX)
#include <osal/linux/OS.h>
#endif
