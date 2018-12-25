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
class Value
{
public:
    Value() {}
    virtual ~Value() {};

    virtual bool Deserialize(std::basic_istream<char> & stream) = 0;
    virtual void Serialize(std::basic_ostream<char> & stream, int indentDepth = 0, bool indentInitial = true) const = 0;

    virtual ValueType Type() const = 0;

private:
};
using ValuePtr = std::shared_ptr<Value>;

ValuePtr Parse(std::basic_istream<char> & stream);

inline void PrintTo(std::basic_ostream<char> & stream, const Value & value)
{
    value.Serialize(stream, 0);
}


} // namepsace JSON
