#pragma once

#include "json/exports.h"
#include <iostream>
#include <memory>
#include "osal/Strings.h"

namespace JSON {

enum class ValueType
{
    Null,
    Boolean,
    String,
    Number,
    Object,
    Array,
};
class JSON_EXPORT Value
{
public:
    Value() {}
    virtual ~Value() {};

    virtual bool Deserialize(std::basic_istream<char> & stream) = 0;
    virtual void Serialize(std::basic_ostream<char> & stream, int indentDepth = 0, bool indentInitial = true) const = 0;

    virtual ValueType Type() const = 0;
    virtual void Set(std::shared_ptr<Value> other) = 0;
    virtual std::shared_ptr<Value> Clone() const = 0;

    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const
    {
        Serialize(s, 0);
        return s;
    }
    virtual bool EqualTo(std::shared_ptr<Value> other) const = 0;
    virtual std::shared_ptr<Value> operator[] (const std::string & key);
    virtual std::shared_ptr<Value> operator[] (size_t index);

private:
};
using ValuePtr = std::shared_ptr<Value>;

ValuePtr Parse(std::basic_istream<char> & stream);

template <class Elem, class Traits>
inline void PrintTo(std::basic_ostream<Elem, Traits> & stream, const Value & value)
{
    value.PrintTo(stream);
}

template <class Elem, class Traits>
inline std::basic_ostream<Elem, Traits> & operator << (std::basic_ostream<Elem, Traits> & stream, const Value & value)
{
    value.PrintTo(stream);
    return stream;
}

bool JSON_EXPORT operator == (ValuePtr lhs, ValuePtr rhs);
bool JSON_EXPORT operator != (ValuePtr lhs, ValuePtr rhs);

} // namepsace JSON
