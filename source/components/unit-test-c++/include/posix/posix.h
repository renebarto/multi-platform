#pragma once

#include <sys/stat.h>
#include <string.h>

// The testing::internal::posix namespace holds wrappers for common
// POSIX functions.  These wrappers hide the differences between
// Windows/MSVC and POSIX systems.  Since some compilers define these
// standard functions as macros, the wrapper cannot have the same name
// as the wrapped function.

namespace posix
{

// Functions with a different name on Windows.

typedef struct stat StatStruct;

inline int FileNo(FILE * file)
{
    return fileno(file);
}
inline int IsATTY(int fd)
{
    return isatty(fd);
}
inline int Stat(const char * path, StatStruct & buf)
{
    return stat(path, &buf);
}
inline int StrCaseCmp(const char * s1, const char * s2)
{
    return strcasecmp(s1, s2);
}
inline char * StrDup(const char * src)
{
    return strdup(src);
}
inline int RmDir(const char * dir)
{
    return rmdir(dir);
}
inline bool IsDir(const StatStruct & st)
{
    return S_ISDIR(st.st_mode);
}

inline const char * StrNCpy(char * dest, const char * src, size_t n)
{
    return strncpy(dest, src, n);
}

// ChDir(), FReopen(), FDOpen(), Read(), Write(), Close(), and
// StrError() aren't needed on Windows CE at this time and thus not
// defined there.

inline int ChDir(const char * dir)
{
    return chdir(dir);
}
inline FILE * FOpen(const char * path, const char * mode)
{
    return fopen(path, mode);
}
inline FILE *FReopen(const char * path, const char * mode, FILE * stream)
{
    return freopen(path, mode, stream);
}
inline FILE * FDOpen(int fd, const char * mode)
{
    return fdopen(fd, mode);
}
inline int FClose(FILE * fp)
{
    return fclose(fp);
}
inline int Read(int fd, void * buf, unsigned int count)
{
    return static_cast<int>(read(fd, buf, count));
}
inline int Write(int fd, const void * buf, unsigned int count)
{
    return static_cast<int>(write(fd, buf, count));
}
inline int Close(int fd)
{
    return close(fd);
}
inline const char * StrError(int errnum)
{
    return strerror(errnum);
}
inline const char * GetEnv(const char * name)
{
    return getenv(name);
}

inline void Abort()
{
    abort();
}

}  // namespace posix

