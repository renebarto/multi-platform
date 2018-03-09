#pragma once

#include <limits>
#include <vector>
#include <type_traits>

namespace RE {

template<class Type>
class InputSet
{
public:
    static const
    typename std::enable_if<std::is_integral<Type>::value, Type>::type MaxValue;
    static const InputSet Empty;
    static const InputSet All;

    class Range
    {
    public:
        Type from;
        Type to;

        Range(Type from, Type to)
            : from(from), to(to)
        {}
        Range(Type value)
            : from(value), to(value)
        {}

        Range(Range const & other)
            : from(other.from), to(other.to)
        {}
    };

    InputSet();

    InputSet(InputSet const & other);

    InputSet(InputSet && other);

    InputSet(Type value);
    InputSet(Range range);

    virtual ~InputSet();

    InputSet & operator = (InputSet const & other);

    InputSet & operator = (InputSet && other);

    Type First() const;
    
    size_t Count() const;

    bool Contains(Type value) const;

    void Add(Type value);
    void Add(Range range);

    InputSet & operator &=(InputSet const & value)
    {
        And(value);
        return *this;
    }

    InputSet & operator |=(InputSet const & value)
    {
        Or(value);
        return *this;
    }

    InputSet & operator -=(InputSet const & value)
    {
        Subtract(value);
        return *this;
    }

    friend bool operator ==(InputSet const & lhs, InputSet const & rhs)
    { return lhs.Equals(rhs); }

    friend bool operator !=(InputSet const & lhs, InputSet const & rhs)
    { return !lhs.Equals(rhs); }

    friend InputSet operator &(InputSet const & lhs, InputSet const & rhs)
    {
        InputSet<Type> result(lhs);
        result.And(rhs);
        return result;
    }

    friend InputSet operator |(InputSet const & lhs, InputSet const & rhs)
    {
        InputSet<Type> result(lhs);
        result.Or(rhs);
        return result;
    }

    friend InputSet operator -(InputSet const & lhs, InputSet const & rhs)
    {
        InputSet<Type> result(lhs);
        result.Subtract(rhs);
        return result;
    }

    friend InputSet operator ~(InputSet const & other)
    {
        InputSet<Type> result;
        result.Fill();
        result -= other;
        return result;
    }

    bool Includes(InputSet const & s) const;

    bool Overlaps(InputSet const & s) const;

    void Clear();

    void Fill();

    using Container = std::vector<Range>;
    using Iterator = typename Container::iterator;
    using ConstIterator = typename Container::const_iterator;

    ConstIterator begin() const
    { return _ranges.begin(); }

    ConstIterator end() const
    { return _ranges.end(); }

private:
    Container _ranges;

    bool Equals(InputSet const & s) const;

    void Or(InputSet const & s);

    void And(InputSet const & s);

    void Subtract(InputSet const & s);

    bool RangesTouch(Iterator left, Iterator right);

    void MergeRanges(Iterator left, Iterator right);
};

template<class Type>
InputSet<Type>::InputSet()
    : _ranges()
{
}

template<class Type>
InputSet<Type>::InputSet(InputSet const & other)
    : _ranges()
{
    for (auto & range : other)
    {
        Range r(range);
        _ranges.push_back(r);
    }
}

template<class Type>
InputSet<Type>::InputSet(InputSet && other)
    : _ranges()
{
    this->_ranges = std::move(other._ranges);
    other._ranges = {};
}

template<class Type>
InputSet<Type>::InputSet(Type value)
    : _ranges()
{
    Add(value);
}

template<class Type>
InputSet<Type>::InputSet(Range range)
    : _ranges()
{
    Add(range);
}


template<class Type>
InputSet<Type>::~InputSet()
{
    Clear();
}

template<class Type>
InputSet<Type> & InputSet<Type>::operator =(InputSet<Type> const & other)
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

template<class Type>
InputSet<Type> & InputSet<Type>::operator =(InputSet && other)
{
    this->_ranges = std::move(other._ranges);
    other._ranges = {};
    return *this;
}

template<class Type>
bool InputSet<Type>::Contains(Type value) const
{
    for (auto & range : _ranges)
    {
        if (value < range.from)
            return false;
        else if (value <= range.to)
            return true; // p.from <= i <= p.to
    }
    return false;
}

template<class Type>
void InputSet<Type>::Add(Type value)
{
    Iterator cur = _ranges.begin();
    while (cur != _ranges.end() && value >= cur->from - 1)
    {
        if (value <= cur->to + 1)
        {
            if (value == cur->from - 1) cur->from--;
            else if (value == cur->to + 1)
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
    Range n(value, value);
    _ranges.insert(cur, n);
}

template<class Type>
void InputSet<Type>::Add(Range range)
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

template<class Type>
bool InputSet<Type>::Equals(InputSet const & s) const
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

template<class Type>
Type InputSet<Type>::First() const
{
    if (_ranges.begin() != _ranges.end())
        return _ranges.begin()->from;
    return MaxValue;
}

template<class Type>
size_t InputSet<Type>::Count() const
{
    size_t n = 0;
    for (auto & range : _ranges)
        n += range.to - range.from + 1;
    return n;
}

template<class Type>
void InputSet<Type>::Or(InputSet const & s)
{
    for (auto & range : s._ranges)
        for (Type i = range.from; i <= range.to; i++)
            Add(i);
}

template<class Type>
void InputSet<Type>::And(InputSet const & s)
{
    InputSet newSet;
    for (auto & range : _ranges)
    {
        for (Type i = range.from; i <= range.to; i++)
            if (s.Contains(i))
                newSet.Add(i);
    }
    _ranges = std::move(newSet._ranges);
    newSet._ranges = {};
}

template<class Type>
void InputSet<Type>::Subtract(InputSet const & s)
{
    InputSet newSet;
    for (auto & range : _ranges)
    {
        for (Type i = range.from; (i <= range.to) && (i >= range.from); i++)
            if (!s.Contains(i))
                newSet.Add(i);
    }
    _ranges = std::move(newSet._ranges);
    newSet._ranges = {};
}

template<class Type>
bool InputSet<Type>::Includes(InputSet const & s) const
{
    for (auto & range : s._ranges)
        for (int i = range.from; i <= range.to; i++)
            if (!Contains(i))
                return false;
    return true;
}

template<class Type>
bool InputSet<Type>::Overlaps(InputSet const & s) const
{
    for (auto & range : s._ranges)
        for (int i = range.from; i <= range.to; i++)
            if (Contains(i))
                return true;
    return false;
}

template<class Type>
void InputSet<Type>::Clear()
{
    _ranges.clear();
}

template<class Type>
void InputSet<Type>::Fill()
{
    Clear();
    _ranges.push_back(Range(0, MaxValue));
}

template<class Type>
bool
InputSet<Type>::RangesTouch(typename InputSet<Type>::Iterator left, typename InputSet<Type>::Iterator right)
{
    return (right != _ranges.end() && left->to >= right->from - 1);
}

template<class Type>
void
InputSet<Type>::MergeRanges(typename InputSet<Type>::Iterator left, typename InputSet<Type>::Iterator right)
{
    left->to = right->to;
    _ranges.erase(right);
}

// This contant is only defined for integral values to guard against incorrect usage
template<class Type>
const
typename std::enable_if<std::is_integral<Type>::value, Type>::type
InputSet<Type>::MaxValue = std::numeric_limits<Type>::max();

template<class Type>
const InputSet<Type> InputSet<Type>::Empty = InputSet<Type>();

// Only support positive _ranges
template<class Type>
const InputSet<Type> InputSet<Type>::All = InputSet<Type>(InputSet<Type>::Range(0, std::numeric_limits<Type>::max()));

} // namespace RE
