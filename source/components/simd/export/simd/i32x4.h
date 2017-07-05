#pragma once

#include <cstdint>

// From Rust std::simd
namespace Simd
{

class i32x4
{
public:
    i32x4()
        : _s1()
        , _s2()
        , _s3()
        , _s4()
    {}
    i32x4(int32_t s1, int32_t s2, int32_t s3, int32_t s4)
        : _s1(s1)
        , _s2(s2)
        , _s3(s3)
        , _s4(s4)
    {}
    i32x4(const i32x4 & other)
        : _s1(other._s1)
        , _s2(other._s2)
        , _s3(other._s3)
        , _s4(other._s4)
    {}

    i32x4 & operator = (const i32x4 & other)
    {
        return Copy(other);
    }

    i32x4 Clone()
    {
        return i32x4(*this);
    }
    i32x4 & Copy(const i32x4 & other)
    {
        if (&other != this)
        {
            _s1 = other._s1;
            _s2 = other._s2;
            _s3 = other._s3;
            _s4 = other._s4;
        }
        return *this;
    }
    i32x4 & Add(const i32x4 & rhs)
    {
        return *this;
    }
    i32x4 & Sub(const i32x4 & rhs)
    {
        return *this;
    }
    i32x4 & Mul(const i32x4 & rhs)
    {
        return *this;
    }
    i32x4 & BitAnd(const i32x4 & rhs)
    {
        return *this;
    }
    i32x4 & BitOr(const i32x4 & rhs)
    {
        return *this;
    }
    i32x4 & BitXor(const i32x4 & rhs)
    {
        return *this;
    }
    i32x4 & Shl(const int32_t rhs)
    {
        return *this;
    }
    i32x4 & Shr(const int32_t rhs)
    {
        return *this;
    }
    i32x4 & operator += (const i32x4 & rhs)
    {
        Add(rhs);
        return *this;
    }
    i32x4 & operator -= (const i32x4 & rhs)
    {
        Sub(rhs);
        return *this;
    }
    i32x4 & operator *= (const i32x4 & rhs)
    {
        Mul(rhs);
        return *this;
    }
    i32x4 & operator &= (const i32x4 & rhs)
    {
        BitAnd(rhs);
        return *this;
    }
    i32x4 & operator |= (const i32x4 & rhs)
    {
        BitOr(rhs);
        return *this;
    }
    i32x4 & operator ^= (const i32x4 & rhs)
    {
        BitXor(rhs);
        return *this;
    }
    i32x4 & operator <<= (const int32_t rhs)
    {
        Shl(rhs);
        return *this;
    }
    i32x4 & operator >>= (const int32_t rhs)
    {
        Shr(rhs);
        return *this;
    }

private:
    int32_t _s1;
    int32_t _s2;
    int32_t _s3;
    int32_t _s4;
};

    i32x4 operator + (const i32x4 & lhs, const i32x4 & rhs)
    {
        i32x4 result(lhs);
        result.Add(rhs);
        return result;
    }
    i32x4 operator - (const i32x4 & lhs, const i32x4 & rhs)
    {
        i32x4 result(lhs);
        result.Sub(rhs);
        return result;
    }
    i32x4 operator * (const i32x4 & lhs, const i32x4 & rhs)
    {
        i32x4 result(lhs);
        result.Mul(rhs);
        return result;
    }
    i32x4 operator & (const i32x4 & lhs, const i32x4 & rhs)
    {
        i32x4 result(lhs);
        result.BitAnd(rhs);
        return result;
    }
    i32x4 operator | (const i32x4 & lhs, const i32x4 & rhs)
    {
        i32x4 result(lhs);
        result.BitOr(rhs);
        return result;
    }
    i32x4 operator ^ (const i32x4 & lhs, const i32x4 & rhs)
    {
        i32x4 result(lhs);
        result.BitXor(rhs);
        return result;
    }
    i32x4 operator << (const i32x4 & lhs, const int32_t rhs)
    {
        i32x4 result(lhs);
        result.Shl(rhs);
        return result;
    }
    i32x4 operator >> (const i32x4 & lhs, const int32_t rhs)
    {
        i32x4 result(lhs);
        result.Shr(rhs);
        return result;
    }

} // namespace Simd
