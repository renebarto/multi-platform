#pragma once

#include <string>
#include "CharSet.h"

namespace RE {

class Term
{
public:
    static constexpr int Any = -1;
    enum Type
    {
        None,
        Literal,        // a
        Set,            // [ab]
        NotSet,         // [^ab]
        Digit,          // \d
        NotDigit,       // \D
        WordChar,       // \w
        NotWordChar,    // \W
        Whitespace,     // \s
        NotWhitespace,  // \S
    };
    Term()
        : _type(Type::None)
        , _charSet()
        , _minCount(1)
        , _maxCount(1)
    {}
    Term(Type type)
        : _type(type)
        , _charSet()
        , _minCount(1)
        , _maxCount(1)
    {}
    Term(Type type, char ch)
        : _type(type)
        , _charSet(ch)
        , _minCount(1)
        , _maxCount(1)
    {}
    Term(Type type, char ch, int minCount, int maxCount)
        : _type(type)
        , _charSet(ch)
        , _minCount(minCount)
        , _maxCount(maxCount)
    {}
    Term(Type type, CharSet charSet)
        : _type(type)
        , _charSet(charSet)
        , _minCount(1)
        , _maxCount(1)
    {}
    Term(Type type, CharSet charSet, int minCount, int maxCount)
        : _type(type)
        , _charSet(charSet)
        , _minCount(minCount)
        , _maxCount(maxCount)
    {}
    Type GetType() const { return _type; }
    void SetMinMaxCount(int minCount, int maxCount)
    {
        _minCount = minCount;
        _maxCount = maxCount;
    }

    friend bool operator == (const Term & lhs, const Term & rhs)
    {
        return (lhs._type == rhs._type) &&
               (lhs._charSet == rhs._charSet) &&
               (lhs._minCount == rhs._minCount) &&
               (lhs._maxCount == rhs._maxCount);
    }
    friend bool operator != (const Term & lhs, const Term & rhs)
    {
        return ! operator == (lhs, rhs);
    }
    void PrintTo(std::ostream & stream) const
    {
        switch (_type)
        {
            case None:
                stream << "Invalid term"; break;
            case Literal:
                stream << "Literal " << _charSet; break;
            case Set:
                stream << "OneOf " << _charSet; break;
            case NotSet:
                stream << "OneNotOf " << _charSet; break;
            case Digit:
                stream << "Digit " << _charSet; break;
            case NotDigit:
                stream << "NotDigit " << _charSet; break;
            case WordChar:
                stream << "WordChar " << _charSet; break;
            case NotWordChar:
                stream << "NotWordChar " << _charSet; break;
            case Whitespace:
                stream << "Whitespace " << _charSet; break;
            case NotWhitespace:
                stream << "NotWhitespace " << _charSet; break;
        }
        stream << " " << "{" << _minCount << "," << _maxCount << "}";
    }

protected:
    Type _type;
    CharSet _charSet;
    int _minCount;
    int _maxCount;
};

inline void PrintTo(const Term & term, std::ostream & stream)
{
    term.PrintTo(stream);
}

} // namespace RE

inline std::ostream & operator << (std::ostream & stream, const RE::Term & term)
{
    term.PrintTo(stream);
    return stream;
}

