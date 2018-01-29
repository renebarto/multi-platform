#pragma once

#define DEFINE_FLAG_OPERATORS(T, B) \
inline T operator | (const T & lhs, const T & rhs) \
{ \
    return T(B(lhs) | B(rhs)); \
} \
inline T operator | (const T & lhs, const B & rhs) \
{ \
    return T(B(lhs) | rhs); \
} \
inline T operator | (const B & lhs, const T & rhs) \
{ \
    return T(lhs | B(rhs)); \
} \
inline T operator & (const T & lhs, const T & rhs) \
{ \
    return T(B(lhs) & B(rhs)); \
} \
inline T operator & (const T & lhs, const B & rhs) \
{ \
    return T(B(lhs) & rhs); \
} \
inline T operator & (const B & lhs, const T & rhs) \
{ \
    return T(lhs & B(rhs)); \
} \
inline T operator ^ (const T & lhs, const T & rhs) \
{ \
    return T(B(lhs) ^ B(rhs)); \
} \
inline T operator ^ (const T & lhs, const B & rhs) \
{ \
    return T(B(lhs) ^ rhs); \
} \
inline T operator ^ (const B & lhs, const T & rhs) \
{ \
    return T(lhs ^ B(rhs)); \
} \
inline T & operator |= (T & lhs, const T & rhs) \
{ \
    lhs = T(B(lhs) | B(rhs)); return lhs; \
} \
inline T & operator |= (T & lhs, const B & rhs) \
{ \
    lhs = T(B(lhs) | rhs); return lhs; \
} \
inline T & operator &= (T & lhs, const T & rhs) \
{ \
    lhs = T(B(lhs) & B(rhs)); return lhs; \
} \
inline T & operator &= (T & lhs, const B & rhs) \
{ \
    lhs = T(B(lhs) & rhs); return lhs; \
} \
inline T & operator ^= (T & lhs, const T & rhs) \
{ \
    lhs = T(B(lhs) ^ B(rhs)); return lhs; \
} \
inline T & operator ^= (T & lhs, const B & rhs) \
{ \
    lhs = T(B(lhs) ^ rhs); return lhs; \
} \
inline T operator ~ (const T & flags) \
{ \
    return T(~B(flags)); \
} \
inline bool operator ! (const T & flags) \
{ \
    return B(flags) == 0; \
} \
inline bool operator == (const B & lhs, const T & rhs) \
{ \
    return lhs == B(rhs); \
} \
inline bool operator == (const T & lhs, const B & rhs) \
{ \
    return B(lhs) == rhs; \
} \
inline bool operator != (const B & lhs, const T & rhs) \
{ \
    return lhs != B(rhs); \
} \
inline bool operator != (const T & lhs, const B & rhs) \
{ \
    return B(lhs) != rhs; \
}
