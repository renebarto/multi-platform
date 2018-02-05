#pragma once

#include <osal/exports.h>

#if defined(WIN_MSVC)
#include "osal/windows/Files.h"
#elif defined(WIN_MINGW)
#include "osal/mingw/Files.h"
#elif defined(DARWIN)
#include "osal/darwin/Files.h"
#elif defined(LINUX)
#include "osal/linux/Files.h"
#endif

namespace OSAL {
namespace Files {

class OSAL_EXPORT TemporaryFile
{
public:
    TemporaryFile();
    ~TemporaryFile();
    void Close();
    std::iostream * GetStream();
    std::string const & GetPath() const;

private:
    std::shared_ptr<std::iostream> _stream;
    std::string _path;
};

} // namespace Files
} // namespace OSAL