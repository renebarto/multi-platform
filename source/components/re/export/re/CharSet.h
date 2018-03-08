#pragma once

#include <deque>
#include <limits>
#include "FSA.h"

namespace RE {

template<class CharType>
class CharSet
    : public InputSet<CharType>
{
public:
    static const CharType CharMax;

    class CharRange
    {
    public:
        CharType from;
        CharType to;

        CharRange(CharType from, CharType to)
            : from(from), to(to)
        {}

        CharRange(CharRange const & other)
            : from(other.from), to(other.to)
        {}
    };

    CharSet();

    CharSet(CharSet const & other);

    CharSet(CharSet && other);

    CharSet(CharType ch);
    CharSet(CharRange range);

    virtual ~CharSet();

    CharSet & operator =(CharSet const & other);

    CharSet & operator =(CharSet && other);

    size_t Count() const;

    bool Contains(CharType ch) const;

    void Add(CharType ch);
    void Add(CharRange range);

    CharSet & operator &=(CharSet const & value)
    {
        And(value);
        return *this;
    }

    CharSet & operator |=(CharSet const & value)
    {
        Or(value);
        return *this;
    }

    CharSet & operator -=(CharSet const & value)
    {
        Subtract(value);
        return *this;
    }

    friend bool operator ==(CharSet const & lhs, CharSet const & rhs)
    { return lhs.Equals(rhs); }

    friend bool operator !=(CharSet const & lhs, CharSet const & rhs)
    { return !lhs.Equals(rhs); }

    friend CharSet operator &(CharSet const & lhs, CharSet const & rhs)
    {
        CharSet<CharType> result(lhs);
        result.And(rhs);
        return result;
    }

    friend CharSet operator |(CharSet const & lhs, CharSet const & rhs)
    {
        CharSet<CharType> result(lhs);
        result.Or(rhs);
        return result;
    }

    friend CharSet operator -(CharSet const & lhs, CharSet const & rhs)
    {
        CharSet<CharType> result(lhs);
        result.Subtract(rhs);
        return result;
    }

    CharType First() const;

    bool Includes(CharSet const & s) const;

    bool Overlaps(CharSet const & s) const;

    void Clear();

    void Fill();

    using Container = std::vector<CharRange>;
    using Iterator = typename Container::iterator;
    using ConstIterator = typename Container::const_iterator;

    ConstIterator begin() const
    { return ranges.begin(); }

    ConstIterator end() const
    { return ranges.end(); }

private:
    Container ranges;

    bool Equals(CharSet const & s) const;

    void Or(CharSet const & s);

    void And(CharSet const & s);

    void Subtract(CharSet const & s);

    bool RangesTouch(Iterator left, Iterator right);

    void MergeRanges(Iterator left, Iterator right);
};

template<class CharType>
CharSet<CharType>::CharSet()
    : InputSet<CharType>()
    , ranges()
{
}

template<class CharType>
CharSet<CharType>::CharSet(CharSet const & other)
    : InputSet<CharType>()
    , ranges()
{
    for (auto & range : other)
    {
        CharRange r(range);
        ranges.push_back(r);
    }
}

template<class CharType>
CharSet<CharType>::CharSet(CharSet && other)
    : InputSet<CharType>()
    , ranges()
{
    this->ranges = std::move(other.ranges);
    other.ranges = {};
}

template<class CharType>
CharSet<CharType>::CharSet(CharType ch)
    : InputSet<CharType>()
    , ranges()
{
    Add(ch);
}

template<class CharType>
CharSet<CharType>::CharSet(CharRange range)
    : InputSet<CharType>()
    , ranges()
{
    Add(range);
}


template<class CharType>
CharSet<CharType>::~CharSet()
{
    Clear();
}

template<class CharType>
CharSet<CharType> & CharSet<CharType>::operator =(CharSet<CharType> const & other)
{
    if (this != &other)
    {
        Clear();
        for (auto & range : other.ranges)
        {
            ranges.emplace_back(range);
        }
    }
    return *this;
}

template<class CharType>
CharSet<CharType> & CharSet<CharType>::operator =(CharSet && other)
{
    this->ranges = std::move(other.ranges);
    other.ranges = {};
    return *this;
}

template<class CharType>
bool CharSet<CharType>::Contains(CharType ch) const
{
    for (auto & range : ranges)
    {
        if (ch < range.from)
            return false;
        else if (ch <= range.to)
            return true; // p.from <= i <= p.to
    }
    return false;
}

template<class CharType>
void CharSet<CharType>::Add(CharType ch)
{
    Iterator cur = ranges.begin();
    while (cur != ranges.end() && ch >= cur->from - 1)
    {
        if (ch <= cur->to + 1)
        {
            if (ch == cur->from - 1) cur->from--;
            else if (ch == cur->to + 1)
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
    CharRange n(ch, ch);
    ranges.insert(cur, n);
}

template<class CharType>
void CharSet<CharType>::Add(CharRange range)
{
    Iterator cur = ranges.begin();
    while (cur != ranges.end() && range.to >= cur->from - 1)
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
    ranges.insert(cur, range);
}

template<class CharType>
bool CharSet<CharType>::Equals(CharSet const & s) const
{
    ConstIterator p = ranges.begin(), q = s.ranges.begin();
    while (p != ranges.end() && q != s.ranges.end())
    {
        if (p->from != q->from || p->to != q->to)
            return false;
        ++p;
        ++q;
    }
    return (p == ranges.end()) && (q == s.ranges.end());
}

template<class CharType>
size_t CharSet<CharType>::Count() const
{
    size_t n = 0;
    for (auto & range : ranges)
        n += range.to - range.from + 1;
    return n;
}

template<class CharType>
CharType CharSet<CharType>::First() const
{
    if (ranges.begin() != ranges.end())
        return ranges.begin()->from;
    return -1;
}

template<class CharType>
void CharSet<CharType>::Or(CharSet const & s)
{
    for (auto & range : s.ranges)
        for (wchar_t i = range.from; i <= range.to; i++)
            Add(i);
}

template<class CharType>
void CharSet<CharType>::And(CharSet const & s)
{
    CharSet newSet;
    for (auto & range : ranges)
    {
        for (wchar_t i = range.from; i <= range.to; i++)
            if (s.Contains(i))
                newSet.Add(i);
    }
    ranges = std::move(newSet.ranges);
    newSet.ranges = {};
}

template<class CharType>
void CharSet<CharType>::Subtract(CharSet const & s)
{
    CharSet newSet;
    for (auto & range : ranges)
    {
        for (wchar_t i = range.from; i <= range.to; i++)
            if (!s.Contains(i))
                newSet.Add(i);
    }
    ranges = std::move(newSet.ranges);
    newSet.ranges = {};
}

template<class CharType>
bool CharSet<CharType>::Includes(CharSet const & s) const
{
    for (auto & range : s.ranges)
        for (int i = range.from; i <= range.to; i++)
            if (!Contains(i))
                return false;
    return true;
}

template<class CharType>
bool CharSet<CharType>::Overlaps(CharSet const & s) const
{
    for (auto & range : s.ranges)
        for (int i = range.from; i <= range.to; i++)
            if (Contains(i))
                return true;
    return false;
}

template<class CharType>
void CharSet<CharType>::Clear()
{
    ranges.clear();
}

template<class CharType>
void CharSet<CharType>::Fill()
{
    Clear();
    ranges.push_back(CharRange(0, CharMax));
}

template<class CharType>
bool
CharSet<CharType>::RangesTouch(typename CharSet<CharType>::Iterator left, typename CharSet<CharType>::Iterator right)
{
    return (right != ranges.end() && left->to >= right->from - 1);
}

template<class CharType>
void
CharSet<CharType>::MergeRanges(typename CharSet<CharType>::Iterator left, typename CharSet<CharType>::Iterator right)
{
    left->to = right->to;
    ranges.erase(right);
}

template<class CharType>
const CharType CharSet<CharType>::CharMax = std::numeric_limits<CharType>::max();

} // namespace RE
