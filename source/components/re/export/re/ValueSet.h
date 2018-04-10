#pragma once

#include <limits>
#include <vector>
#include <type_traits>

namespace RE {

template<class Type, class UnderlyingType>
class ValueSet
{
public:
    static const
    typename std::enable_if<std::is_integral<Type>::value, Type>::type MaxValue;
    static const ValueSet Empty;
    static const ValueSet All;

    class Range
    {
    public:
        UnderlyingType from;
        UnderlyingType to;

        Range(UnderlyingType from, UnderlyingType to)
            : from(from), to(to)
        {}
        Range(UnderlyingType value)
            : from(value), to(value)
        {}

        Range(Range const & other)
            : from(other.from), to(other.to)
        {}
    };

    ValueSet();

    ValueSet(ValueSet const & other);

    ValueSet(ValueSet && other);

    ValueSet(Type value);
    ValueSet(Range range);

    virtual ~ValueSet();

    ValueSet & operator = (ValueSet const & other);

    ValueSet & operator = (ValueSet && other);

    size_t Count() const;

    bool Contains(Type value) const;
    bool IsEmpty() const;
    bool IsFull() const;

    void Add(Type value);
    void Add(Range range);

    ValueSet & operator &=(ValueSet const & value)
    {
        And(value);
        return *this;
    }

    ValueSet & operator |=(ValueSet const & value)
    {
        Or(value);
        return *this;
    }

    ValueSet & operator -=(ValueSet const & value)
    {
        Subtract(value);
        return *this;
    }

    friend bool operator ==(ValueSet const & lhs, ValueSet const & rhs)
    { return lhs.Equals(rhs); }

    friend bool operator !=(ValueSet const & lhs, ValueSet const & rhs)
    { return !lhs.Equals(rhs); }

    friend ValueSet operator &(ValueSet const & lhs, ValueSet const & rhs)
    {
        ValueSet<Type, UnderlyingType> result(lhs);
        result.And(rhs);
        return result;
    }

    friend ValueSet operator |(ValueSet const & lhs, ValueSet const & rhs)
    {
        ValueSet<Type, UnderlyingType> result(lhs);
        result.Or(rhs);
        return result;
    }

    friend ValueSet operator -(ValueSet const & lhs, ValueSet const & rhs)
    {
        ValueSet<Type, UnderlyingType> result(lhs);
        result.Subtract(rhs);
        return result;
    }

    friend ValueSet operator ~(ValueSet const & other)
    {
        ValueSet<Type, UnderlyingType> result;
        result.Fill();
        result -= other;
        return result;
    }

    bool Includes(ValueSet const & s) const;

    bool Overlaps(ValueSet const & s) const;

    void Clear();

    void Fill();

    using Container = std::vector<Range>;
    using Iterator = typename Container::iterator;
    using ConstIterator = typename Container::const_iterator;

    virtual void PrintTo(std::ostream & stream) const
    {
        bool firstElement = true;
        for (auto const & range : _ranges)
        {
            if (!firstElement)
            {
                stream << ",";
            }
            if (range.from == range.to)
                stream << static_cast<Type>(range.from);
            else
                stream << static_cast<Type>(range.from) << "-" << static_cast<Type>(range.to);
            firstElement = false;
        }
    }

protected:
    bool Equals(ValueSet const & s) const;
    void Or(ValueSet const & s);
    void And(ValueSet const & s);
    void Subtract(ValueSet const & s);
    bool RangesTouch(Iterator left, Iterator right);
    void MergeRanges(Iterator left, Iterator right);

    ConstIterator begin() const
    { return _ranges.begin(); }

    ConstIterator end() const
    { return _ranges.end(); }

private:
    Container _ranges;
};

template<class Type, class UnderlyingType>
ValueSet<Type, UnderlyingType>::ValueSet()
    : _ranges()
{
}

template<class Type, class UnderlyingType>
ValueSet<Type, UnderlyingType>::ValueSet(ValueSet const & other)
    : _ranges()
{
    for (auto & range : other)
    {
        Range r(range);
        _ranges.push_back(r);
    }
}

template<class Type, class UnderlyingType>
ValueSet<Type, UnderlyingType>::ValueSet(ValueSet && other)
    : _ranges()
{
    this->_ranges = std::move(other._ranges);
    other._ranges = {};
}

template<class Type, class UnderlyingType>
ValueSet<Type, UnderlyingType>::ValueSet(Type value)
    : _ranges()
{
    Add(value);
}

template<class Type, class UnderlyingType>
ValueSet<Type, UnderlyingType>::ValueSet(Range range)
    : _ranges()
{
    Add(range);
}


template<class Type, class UnderlyingType>
ValueSet<Type, UnderlyingType>::~ValueSet()
{
    Clear();
}

template<class Type, class UnderlyingType>
ValueSet<Type, UnderlyingType> & ValueSet<Type, UnderlyingType>::operator =(ValueSet<Type, UnderlyingType> const & other)
{
    if (this != &other)
    {
        Clear();
        for (auto & range : other._ranges)
        {
            _ranges.emplace_back(range);
        }
    }
    return *this;
}

template<class Type, class UnderlyingType>
ValueSet<Type, UnderlyingType> & ValueSet<Type, UnderlyingType>::operator =(ValueSet && other)
{
    this->_ranges = std::move(other._ranges);
    other._ranges = {};
    return *this;
}

template<class Type, class UnderlyingType>
bool ValueSet<Type, UnderlyingType>::Contains(Type value) const
{
    auto underlyingValue = static_cast<UnderlyingType>(value);
    for (auto & range : _ranges)
    {
        if (underlyingValue < range.from)
            return false;
        else if (underlyingValue <= range.to)
            return true; // p.from <= i <= p.to
    }
    return false;
}

template<class Type, class UnderlyingType>
bool ValueSet<Type, UnderlyingType>::IsEmpty() const
{
    return _ranges.empty();
}

template<class Type, class UnderlyingType>
bool ValueSet<Type, UnderlyingType>::IsFull() const
{
    return (_ranges.size() == size_t{1}) &&
           (_ranges[0].from == 0) &&
           (_ranges[0].to == std::numeric_limits<Type>::max());
}

template<class Type, class UnderlyingType>
void ValueSet<Type, UnderlyingType>::Add(Type value)
{
    auto underlyingValue = static_cast<UnderlyingType>(value);
    Iterator cur = _ranges.begin();
    while (cur != _ranges.end() && underlyingValue >= cur->from - 1)
    {
        if (underlyingValue <= cur->to + 1)
        {
            if (underlyingValue == cur->from - 1) cur->from--;
            else if (underlyingValue == cur->to + 1)
            {
                cur->to++;
                Iterator next = cur + 1;
                if (RangesTouch(cur, next))
                {
                    MergeRanges(cur, next);
                }
            }
            return;
        }
        ++cur;
    }
    Range n(underlyingValue, underlyingValue);
    _ranges.insert(cur, n);
}

template<class Type, class UnderlyingType>
void ValueSet<Type, UnderlyingType>::Add(Range range)
{
    Iterator cur = _ranges.begin();
    while (cur != _ranges.end() && range.to >= cur->from - 1)
    {
        if ((range.from <= cur->to + 1) && ((range.to >= cur->from - 1)))
        {
            if (range.to >= cur->to + 1) cur->to = range.to;
            if (range.from <= cur->from - 1) cur->from = range.from;
            Iterator next = cur + 1;
            if (RangesTouch(cur, next))
            {
                MergeRanges(cur, next);
            }
            return;
        }
        ++cur;
    }
    _ranges.insert(cur, range);
}

template<class Type, class UnderlyingType>
bool ValueSet<Type, UnderlyingType>::Equals(ValueSet const & s) const
{
    ConstIterator p = _ranges.begin(), q = s._ranges.begin();
    while (p != _ranges.end() && q != s._ranges.end())
    {
        if (p->from != q->from || p->to != q->to)
            return false;
        ++p;
        ++q;
    }
    return (p == _ranges.end()) && (q == s._ranges.end());
}

template<class Type, class UnderlyingType>
size_t ValueSet<Type, UnderlyingType>::Count() const
{
    size_t n = 0;
    for (auto & range : _ranges)
        n += range.to - range.from + 1;
    return n;
}

template<class Type, class UnderlyingType>
void ValueSet<Type, UnderlyingType>::Or(ValueSet const & s)
{
    for (auto & range : s._ranges)
        for (auto i = range.from; i <= range.to; i++)
        {
            auto u = static_cast<Type>(i);
            Add(u);
        }
}

template<class Type, class UnderlyingType>
void ValueSet<Type, UnderlyingType>::And(ValueSet const & s)
{
    ValueSet newSet;
    for (auto & range : _ranges)
    {
        for (auto i = range.from; i <= range.to; i++)
        {
            auto u = static_cast<Type>(i);
            if (s.Contains(u))
                newSet.Add(u);
        }
    }
    _ranges = std::move(newSet._ranges);
    newSet._ranges = {};
}

template<class Type, class UnderlyingType>
void ValueSet<Type, UnderlyingType>::Subtract(ValueSet const & s)
{
    ValueSet newSet;
    for (auto & range : _ranges)
    {
        for (auto i = range.from; (i <= range.to) && (i >= range.from); i++)
        {
            auto u = static_cast<Type>(i);
            if (!s.Contains(u))
                newSet.Add(u);
        }
    }
    _ranges = std::move(newSet._ranges);
    newSet._ranges = {};
}

template<class Type, class UnderlyingType>
bool ValueSet<Type, UnderlyingType>::Includes(ValueSet const & s) const
{
    for (auto & range : s._ranges)
        for (auto i = range.from; i <= range.to; i++)
        {
            auto u = static_cast<Type>(i);
            if (!Contains(u))
                return false;
        }
    return true;
}

template<class Type, class UnderlyingType>
bool ValueSet<Type, UnderlyingType>::Overlaps(ValueSet const & s) const
{
    for (auto & range : s._ranges)
        for (auto i = range.from; i <= range.to; i++)
        {
            auto u = static_cast<Type>(i);
            if (Contains(u))
                return true;
        }
    return false;
}

template<class Type, class UnderlyingType>
void ValueSet<Type, UnderlyingType>::Clear()
{
    _ranges.clear();
}

template<class Type, class UnderlyingType>
void ValueSet<Type, UnderlyingType>::Fill()
{
    Clear();
    _ranges.push_back(Range(0, MaxValue));
}

template<class Type, class UnderlyingType>
bool ValueSet<Type, UnderlyingType>::RangesTouch(typename ValueSet<Type, UnderlyingType>::Iterator left,
                                                 typename ValueSet<Type, UnderlyingType>::Iterator right)
{
    return (right != _ranges.end() && left->to >= right->from - 1);
}

template<class Type, class UnderlyingType>
void ValueSet<Type, UnderlyingType>::MergeRanges(typename ValueSet<Type, UnderlyingType>::Iterator left,
                                                 typename ValueSet<Type, UnderlyingType>::Iterator right)
{
    left->to = right->to;
    _ranges.erase(right);
}

// This contant is only defined for integral values to guard against incorrect usage
template<class Type, class UnderlyingType>
const typename std::enable_if<std::is_integral<Type>::value, Type>::type
ValueSet<Type, UnderlyingType>::MaxValue = std::numeric_limits<Type>::max();

template<class Type, class UnderlyingType>
const ValueSet<Type, UnderlyingType> ValueSet<Type, UnderlyingType>::Empty = ValueSet<Type, UnderlyingType>();

// Only support positive _ranges
template<class Type, class UnderlyingType>
const ValueSet<Type, UnderlyingType> ValueSet<Type, UnderlyingType>::All =
    ValueSet<Type, UnderlyingType>(ValueSet<Type, UnderlyingType>::Range(0, std::numeric_limits<Type>::max()));

template<class Type, class UnderlyingType>
inline void PrintTo(const ValueSet<Type, UnderlyingType> & inputSet, std::ostream & stream)
{
    inputSet.PrintTo(stream);
}

} // namespace RE

template<class Type, class UnderlyingType>
inline std::ostream & operator << (std::ostream & stream, const RE::ValueSet<Type, UnderlyingType> & inputSet)
{
    inputSet.PrintTo(stream);
    return stream;
}
