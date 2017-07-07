#pragma once

#define WARNING_PUSH _Pragma("clang diagnostic push")
#define WARNING_PUSH_N(level) WARNING_PUSH
#define WARNING_DISABLE_UNUSED_PARAMETER _Pragma("clang diagnostic ignored \"-Wunused-parameter\"")
#define WARNING_DISABLE(num)
#define WARNING_POP _Pragma("clang diagnostic pop")

