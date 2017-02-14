#pragma once

// Standard operators for integer type derived enum class types.

//template<typename T, class Enable = void>
//class Flag {}; // primary template
//
//template<typename T>
//class Flag<T, typename std::enable_if<std::is_enum<T>::value>::type>: public T
//{
//public:
//    Flag() {}
//
//};

#define DEFINE_FLAG_OPERATORS(T, B) \
inline T operator | (const T & lhs, const T & rhs) \
{ \
    return T(B(lhs) | B(rhs)); \
} \
inline T operator & (const T & lhs, const T & rhs) \
{ \
    return T(B(lhs) & B(rhs)); \
} \
inline T operator ^ (const T & lhs, const T & rhs) \
{ \
    return T(B(lhs) ^ B(rhs)); \
} \
inline T & operator |= (T & lhs, const T & rhs) \
{ \
    lhs = T(B(lhs) | B(rhs)); \
    return lhs; \
} \
inline T & operator &= (T & lhs, const T & rhs) \
{ \
    lhs = T(B(lhs) & B(rhs)); \
    return lhs; \
} \
inline T & operator ^= (T & lhs, const T & rhs) \
{ \
    lhs = T(B(lhs) ^ B(rhs)); \
    return lhs; \
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
inline bool operator != (const B & lhs, const T & rhs) \
{ \
    return lhs != B(rhs); \
} \
inline bool operator == (const T & lhs, const B & rhs) \
{ \
    return B(lhs) == rhs; \
} \
inline bool operator != (const T & lhs, const B & rhs) \
{ \
    return B(lhs) != rhs; \
}
