#include "osal/PlatformDefines.h"

#if defined(LINUX)

#include "osal/Path.h"
#include "osal/SharedLibrary.h"

using namespace std;
using namespace OSAL;

SharedLibrary::SharedLibrary()
    : _handle()
    , _flags(OpenFlags::Immediate)
    , _fileName()
{
    _handle = dlopen(nullptr, static_cast<int>(_flags));
}

SharedLibrary::SharedLibrary(const std::string & path)
    : _handle()
    , _flags(OpenFlags::Immediate)
    , _fileName(path)
{
    _handle = dlopen(path.c_str(), static_cast<int>(_flags));
}

SharedLibrary::SharedLibrary(const std::string & path, OpenFlags flags)
    : _handle()
    , _flags(flags)
{
    _handle = dlopen(path.c_str(), static_cast<int>(_flags));
}

SharedLibrary::~SharedLibrary()
{
    if (_handle != nullptr)
        dlclose(_handle);
}

bool SharedLibrary::IsLoaded() const
{
    return _handle != nullptr;
}

char * SharedLibrary::Error()
{
    return dlerror();
}

void * SharedLibrary::FindSymbol(FindFlags flags, const std::string & name, bool & found)
{
    dlerror();
    void * result = dlsym(reinterpret_cast<void *>(static_cast<int>(flags)), name.c_str());
    if (!result && dlerror())
    {
        found = false;
        return nullptr;
    }
    found = true;
    return result;
}

void * SharedLibrary::FindSymbol(const std::string & name)
{
    bool found;
    return FindSymbol(name, found);
}

void * SharedLibrary::FindSymbol(const std::string & name, bool & found)
{
    dlerror();
    void * result = dlsym(_handle, name.c_str());
    if (!result && dlerror())
    {
        found = false;
        return nullptr;
    }
    found = true;
    return result;
}

std::string SharedLibrary::GetDirectory() const
{
    char buffer[4096];
    if (dlinfo(_handle, RTLD_DI_ORIGIN, buffer) == 0)
        return string(buffer);
    return string();
}

std::string SharedLibrary::GetFileName() const
{
    if (!_fileName.empty())
        return _fileName;
    const size_t BufferSize = 4096;
    char buffer[BufferSize];
    readlink("/proc/self/exe", buffer, BufferSize);
    return Path::LastPartOfPath(buffer);
}

std::string SharedLibrary::GetFullPath() const
{
    return Path::CombinePath(GetDirectory(), GetFileName());
}

#endif // defined(LINUX)
