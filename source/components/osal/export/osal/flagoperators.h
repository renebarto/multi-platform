#pragma once

#include <osal/strings.h>

// Standard operators for integer type derived enum class types.
template<typename T, typename B, class Enable = void>
class Flag {}; // primary template

template<typename T, typename B>
class Flag<T, B, typename std::enable_if<std::is_enum<T>::value>::type>
{
public:
    Flag()
        : _value()
    {}
    Flag(const Flag & value)
        : _value(value._value)
    {}
    Flag(const T & value)
        : _value(value)
    {}
    Flag(const B & value)
        : _value(static_cast<T>(value))
    {}
    Flag & operator = (const Flag & value)
    {
        if (this != &value)
        {
            _value = value._value;
        }
        return *this;
    }
    Flag & operator = (const T & value)
    {
        _value = value;
        return *this;
    }
    Flag & operator = (const B & value)
    {
        _value = static_cast<T>(value);
        return *this;
    }

    Flag<T, B> operator | (const Flag<T, B> & rhs) const
    {
        return Flag<T, B>(static_cast<B>(_value) | static_cast<B>(rhs._value));
    }
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator | (const Flag<TT, BB> & lhs, const TT & rhs);
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator | (const TT & lhs, const Flag<TT, BB> & rhs);
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator | (const Flag<TT, BB> & lhs, const BB & rhs);
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator | (const BB & lhs, const Flag<TT, BB> & rhs);
    Flag<T, B> operator & (const Flag<T, B> & rhs) const
    {
        return Flag<T, B>(static_cast<B>(_value) & static_cast<B>(rhs._value));
    }
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator & (const Flag<TT, BB> & lhs, const TT & rhs);
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator & (const TT & lhs, const Flag<TT, BB> & rhs);
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator & (const Flag<TT, BB> & lhs, const BB & rhs);
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator & (const BB & lhs, const Flag<TT, BB> & rhs);
    Flag<T, B> operator ^ (const Flag<T, B> & rhs) const
    {
        return Flag<T, B>(static_cast<B>(_value) ^ static_cast<B>(rhs._value));
    }
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator ^ (const Flag<TT, BB> & lhs, const TT & rhs);
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator ^ (const TT & lhs, const Flag<TT, BB> & rhs);
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator ^ (const Flag<TT, BB> & lhs, const BB & rhs);
    template<typename TT, typename BB>
    friend Flag<TT, BB> operator ^ (const BB & lhs, const Flag<TT, BB> & rhs);
    T & operator |= (const Flag<T, B> & rhs)
    {
        _value = static_cast<T>(static_cast<B>(_value) | static_cast<B>(rhs._value));
        return _value;
    }
    T & operator |= (const T & rhs)
    {
        _value = static_cast<T>(static_cast<B>(_value) | static_cast<B>(rhs));
        return _value;
    }
    T & operator |= (const B & rhs)
    {
        _value = static_cast<T>(static_cast<B>(_value) | rhs);
        return _value;
    }
    T & operator &= (const Flag<T, B> & rhs)
    {
        _value = static_cast<T>(static_cast<B>(_value) & static_cast<B>(rhs._value));
        return _value;
    }
    T & operator &= (const T & rhs)
    {
        _value = static_cast<T>(static_cast<B>(_value) & static_cast<B>(rhs));
        return _value;
    }
    T & operator &= (const B & rhs)
    {
        _value = static_cast<T>(static_cast<B>(_value) & rhs);
        return _value;
    }
    T & operator ^= (const Flag<T, B> & rhs)
    {
        _value = static_cast<T>(static_cast<B>(_value) ^ static_cast<B>(rhs._value));
        return _value;
    }
    T & operator ^= (const T & rhs)
    {
        _value = static_cast<T>(static_cast<B>(_value) ^ static_cast<B>(rhs));
        return _value;
    }
    T & operator ^= (const B & rhs)
    {
        _value = static_cast<T>(static_cast<B>(_value) ^ rhs);
        return _value;
    }
    inline Flag<T, B> operator ~ () const
    {
        return static_cast<T>(~static_cast<B>(_value));
    }
    inline bool operator ! () const
    {
        return static_cast<B>(_value) == 0;
    }
    // This is some trickery to prevent using a normal bool operator.
    // The bool operator can be silently promoted to int, leading to ambiguous comparison operator solutions.
    typedef void (Flag::*safe_bool)() const;
    void inhibit_safe_bool_comparison() const {}
    inline operator safe_bool () const
    {
        return (static_cast<B>(_value) != 0) ? &Flag::inhibit_safe_bool_comparison : nullptr;
    }
    bool operator == (const Flag<T, B> & rhs) const
    {
        return static_cast<B>(_value) == static_cast<B>(rhs._value);
    }
    friend inline bool operator == (const Flag<T, B> & lhs, const T & rhs)
    {
        return static_cast<B>(lhs._value) == static_cast<B>(rhs);
    }
    friend inline bool operator == (const T & lhs, const Flag<T, B> & rhs)
    {
        return static_cast<B>(lhs) == static_cast<B>(rhs._value);
    }
    friend inline bool operator == (const Flag<T, B> & lhs, const B & rhs)
    {
        return static_cast<B>(lhs._value) == rhs;
    }
    friend inline bool operator == (const B & lhs, const Flag<T, B> & rhs)
    {
        return lhs == static_cast<B>(rhs._value);
    }
    friend inline bool operator == (const B & lhs, const T & rhs)
    {
        return lhs == B(rhs);
    }
    friend inline bool operator == (const T & lhs, const B & rhs)
    {
        return B(lhs) == rhs;
    }
    bool operator != (const Flag<T, B> & rhs) const
    {
        return static_cast<B>(_value) != static_cast<B>(rhs._value);
    }
    friend inline bool operator != (const Flag<T, B> & lhs, const T & rhs)
    {
        return static_cast<B>(lhs._value) != static_cast<B>(rhs);
    }
    friend inline bool operator != (const T & lhs, const Flag<T, B> & rhs)
    {
        return static_cast<B>(lhs) != static_cast<B>(rhs._value);
    }
    friend inline bool operator != (const Flag<T, B> & lhs, const B & rhs)
    {
        return static_cast<B>(lhs._value) != rhs;
    }
    friend inline bool operator != (const B & lhs, const Flag<T, B> & rhs)
    {
        return lhs != static_cast<B>(rhs._value);
    }
    friend inline bool operator != (const B & lhs, const T & rhs)
    {
        return lhs != B(rhs);
    }
    friend inline bool operator != (const T & lhs, const B & rhs)
    {
        return B(lhs) != rhs;
    }
    T Value() const { return _value; }
    operator T () const { return _value; }

private:
    T _value;
};

template<typename T, typename B>
inline Flag<T, B> operator | (const Flag<T, B> & lhs, const T & rhs)
{
    return Flag<T, B>(static_cast<B>(lhs._value) | static_cast<B>(rhs));
}
template<typename T, typename B>
inline Flag<T, B> operator | (const T & lhs, const Flag<T, B> & rhs)
{
    return static_cast<T>(static_cast<B>(lhs) | static_cast<B>(rhs._value));
}
template<typename T, typename B>
inline Flag<T, B> operator | (const Flag<T, B> & lhs, const B & rhs)
{
    return static_cast<T>(static_cast<B>(lhs._value) | rhs);
}
template<typename T, typename B>
inline Flag<T, B> operator | (const B & lhs, const Flag<T, B> & rhs)
{
    return lhs | static_cast<B>(rhs._value);
}
template<typename T, typename B>
inline Flag<T, B> operator & (const Flag<T, B> & lhs, const T & rhs)
{
    return Flag<T, B>(static_cast<B>(lhs._value) & static_cast<B>(rhs));
}
template<typename T, typename B>
inline Flag<T, B> operator & (const T & lhs, const Flag<T, B> & rhs)
{
    return static_cast<T>(static_cast<B>(lhs) & static_cast<B>(rhs._value));
}
template<typename T, typename B>
inline Flag<T, B> operator & (const Flag<T, B> & lhs, const B & rhs)
{
    return static_cast<T>(static_cast<B>(lhs._value) & rhs);
}
template<typename T, typename B>
inline Flag<T, B> operator & (const B & lhs, const Flag<T, B> & rhs)
{
    return lhs & static_cast<B>(rhs._value);
}
template<typename T, typename B>
inline Flag<T, B> operator ^ (const Flag<T, B> & lhs, const T & rhs)
{
    return Flag<T, B>(static_cast<B>(lhs._value) ^ static_cast<B>(rhs));
}
template<typename T, typename B>
inline Flag<T, B> operator ^ (const T & lhs, const Flag<T, B> & rhs)
{
    return static_cast<T>(static_cast<B>(lhs) ^ static_cast<B>(rhs._value));
}
template<typename T, typename B>
inline Flag<T, B> operator ^ (const Flag<T, B> & lhs, const B & rhs)
{
    return static_cast<T>(static_cast<B>(lhs._value) ^ rhs);
}
template<typename T, typename B>
inline Flag<T, B> operator ^ (const B & lhs, const Flag<T, B> & rhs)
{
    return lhs ^ static_cast<B>(rhs._value);
}

template<typename T, typename B>
inline std::basic_ostream<OSAL::Char> & operator << (std::basic_ostream<OSAL::Char> & stream, Flag<T, B> value)
{
    stream << value.Value();
    return stream;
}

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
inline T operator ~ (const T & flags) \
{ \
    return T(~B(flags)); \
} \
inline bool operator ! (const T & flags) \
{ \
    return B(flags) == 0; \
}

