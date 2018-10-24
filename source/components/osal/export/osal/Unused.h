#pragma once

#if !defined(UNUSED)
#if defined(__GNUC__)
#define UNUSED(x) x __attribute__((unused))
#elif defined(_MSC_VER)
#define UNUSED(x)
#else
#define UNUSED(x) x
#endif
#endif
