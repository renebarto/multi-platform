#pragma once

#include <dlfcn.h>
#include <string>
#include "osal/exports.h"
#include "osal/FlagOperators.h"

namespace OSAL {

class OSAL_EXPORT SharedLibrary
{
public:
    enum class OpenFlags : int
    {
        LazyBinding = RTLD_LAZY,    // Lazy function call binding.
        Immediate = RTLD_NOW,       // Immediate function call binding.
        NoLoad = RTLD_NOLOAD,       // Do not load the object.
        DeepBinding = RTLD_DEEPBIND,// Use deep binding.
        Global = RTLD_GLOBAL,       // The symbols defined by this shared object will be made available for symbol
                                    // resolution of subsequently loaded shared objects.
        Local = RTLD_LOCAL,         // This is the converse of RTLD_GLOBAL, and the default if neither flag is specified.
        NoDelete = RTLD_NODELETE,   // Do not delete object when closed.
    };
    enum class FindFlags
    {
        Default = 0,                // Find the first occurrence of the desired symbol using the default shared object
                                    // search order.
        Next = -1,                  // Find the next occurrence of the desired symbol in the search order after the
                                    // current object.
    };

    SharedLibrary();
    SharedLibrary(const SharedLibrary &) = delete;
    SharedLibrary(SharedLibrary &&) = delete;
    SharedLibrary(const std::string & path);
    SharedLibrary(const std::string & path, OpenFlags flags);
    virtual ~SharedLibrary();

    SharedLibrary & operator = (const SharedLibrary &) = delete;
    SharedLibrary & operator = (SharedLibrary &&) = delete;

    bool IsLoaded() const;
    char * Error();
    void * FindSymbol(FindFlags flags, const std::string & name, bool & found);
    void * FindSymbol(const std::string & name);
    void * FindSymbol(const std::string & name, bool & found);
    std::string GetDirectory() const;
    std::string GetFileName() const;
    std::string GetFullPath() const;

private:
    void * _handle;
    OpenFlags _flags;
    std::string _fileName;
};

} // namespace OSAL

template<>
struct is_flag<OSAL::SharedLibrary::OpenFlags>
{
    static constexpr bool value=true;
};

