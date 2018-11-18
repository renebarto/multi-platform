#pragma once

namespace OSAL {

template<typename E>
struct is_flag
{
    static constexpr bool value = false;
};

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, flags>::type
    inline operator | (flags lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<flags>(static_cast<underlyingType>(lhs) | static_cast<underlyingType>(rhs));
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, flags>::type
    inline operator | (flags lhs, typename std::underlying_type<flags>::type rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<flags>(static_cast<underlyingType>(lhs) | rhs);
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, flags>::type
    inline operator | (typename std::underlying_type<flags>::type lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<flags>(lhs | static_cast<underlyingType>(rhs));
}

template<typename flags>
typename std::enable_if<is_flag<flags>::value, flags &>::type
    inline operator |= (flags & lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    lhs = lhs | static_cast<underlyingType>(rhs);
    return lhs;
}

template<typename flags>
typename std::enable_if<is_flag<flags>::value, flags &>::type
    inline operator |= (flags &lhs, typename std::underlying_type<flags>::type rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, flags>::type
    inline operator & (flags lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<flags>(static_cast<underlyingType>(lhs) & static_cast<underlyingType>(rhs));
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, flags>::type
    inline operator & (flags lhs, typename std::underlying_type<flags>::type rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<flags>(static_cast<underlyingType>(lhs) & rhs);
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, flags>::type
    inline operator & (typename std::underlying_type<flags>::type lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<flags>(lhs & static_cast<underlyingType>(rhs));
}

template<typename flags>
typename std::enable_if<is_flag<flags>::value, flags &>::type
    inline operator &= (flags & lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    lhs = lhs & static_cast<underlyingType>(rhs);
    return lhs;
}

template<typename flags>
typename std::enable_if<is_flag<flags>::value, flags &>::type
    inline operator &= (flags &lhs, typename std::underlying_type<flags>::type rhs)
{
    lhs = lhs & rhs;
    return lhs;
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, flags>::type
    inline operator ^ (flags lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<flags>(static_cast<underlyingType>(lhs) ^ static_cast<underlyingType>(rhs));
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, flags>::type
    inline operator ^ (flags lhs, typename std::underlying_type<flags>::type rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<flags>(static_cast<underlyingType>(lhs) ^ rhs);
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, flags>::type
    inline operator ^ (typename std::underlying_type<flags>::type lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<flags>(lhs ^ static_cast<underlyingType>(rhs));
}

template<typename flags>
typename std::enable_if<is_flag<flags>::value, flags &>::type
    inline operator ^= (flags & lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    lhs = lhs ^ static_cast<underlyingType>(rhs);
    return lhs;
}

template<typename flags>
typename std::enable_if<is_flag<flags>::value, flags &>::type
    inline operator ^= (flags &lhs, typename std::underlying_type<flags>::type rhs)
{
    lhs = lhs ^ rhs;
    return lhs;
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, flags>::type
    inline operator ~ (flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<flags>(~static_cast<underlyingType>(rhs));
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, bool>::type
    inline operator ! (flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<underlyingType>(rhs) == 0;
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, bool>::type
    inline operator == (flags lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<underlyingType>(lhs) == static_cast<underlyingType>(rhs);
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, bool>::type
    inline operator == (flags lhs, typename std::underlying_type<flags>::type rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<underlyingType>(lhs) == rhs;
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, bool>::type
    inline operator == (typename std::underlying_type<flags>::type lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return lhs == static_cast<underlyingType>(rhs);
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, bool>::type
    inline operator != (flags lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<underlyingType>(lhs) != static_cast<underlyingType>(rhs);
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, bool>::type
    inline operator != (flags lhs, typename std::underlying_type<flags>::type rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return static_cast<underlyingType>(lhs) != rhs;
}

template<typename flags>
typename std::enable_if<std::is_enum<flags>::value && is_flag<flags>::value, bool>::type
    inline operator != (typename std::underlying_type<flags>::type lhs, flags rhs)
{
    typedef typename std::underlying_type<flags>::type underlyingType;

    return lhs != static_cast<underlyingType>(rhs);
}

} // namespace OSAL