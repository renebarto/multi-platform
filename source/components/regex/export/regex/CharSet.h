#pragma once

#include <iomanip>
#include "ValueSet.h"

namespace Regex {

class CharSet : public ValueSet<char, int>
{
public:
    CharSet()
        : ValueSet()
    {}

    CharSet(ValueSet const & other)
        : ValueSet(other)
    {}

    CharSet(CharSet const & other)
        : ValueSet(other)
    {}

    CharSet(CharSet && other)
        : ValueSet(std::move(other))
    {}

    CharSet(char value)
        : ValueSet(value)
    {}
    CharSet(ValueSet::Range range)
        : ValueSet(range)
    {}

    CharSet & operator = (CharSet const & other)
    {
        ValueSet::operator=(other);
        return *this;
    }

    CharSet & operator = (CharSet && other)
    {
        ValueSet::operator=(std::move(other));
        return *this;
    }

    char First() const
    {
        ConstIterator it = begin();
        if (it != end())
        {
            return static_cast<char>(it->from);
        }
        return '\0';
    }
    void PrintTo(std::ostream & stream) const override
    {
        bool firstElement = true;
        for (ConstIterator it = begin(); it != end(); ++it)
        {
            auto const & range = *it;
            if (!firstElement)
            {
                stream << ",";
            }
            if (isprint(range.from))
            {
                stream << static_cast<char>(range.from);
            } else
            {
                stream << "\\x" << std::hex << std::setw(2) << std::setfill('0') << std::dec << range.from;
            }
            if (range.from != range.to)
            {
                stream << "-";
                if (isprint(range.to))
                {
                    stream << static_cast<char>(range.to);
                } else
                {
                    stream << "\\x" << std::hex << std::setw(2) << std::setfill('0') << std::dec << range.to;
                }
            }
            firstElement = false;
        }
    }
};

} // namespace Regex

