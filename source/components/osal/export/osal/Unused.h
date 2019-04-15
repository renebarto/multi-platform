#pragma once

#if !defined(UNUSED)
# if defined(__GNUC__)
#  if defined(__linux__)
#   define UNUSED(x) x __attribute__((unused))
#  else
#   define UNUSED(x)
#  endif
# elif defined(_MSC_VER)
#  define UNUSED(x)
# else
#  define UNUSED(x) x
# endif
#endif
