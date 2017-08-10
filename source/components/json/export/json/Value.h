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
    Value();
    virtual ~Value() {};

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) = 0;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream, int indentDepth = 0) const = 0;

    virtual ValueType Type() const = 0;

private:
};
using ValuePtr = std::shared_ptr<Value>;

ValuePtr Parse(std::basic_istream<OSAL::Char> & stream);

inline void PrintTo(std::basic_ostream<OSAL::Char> & stream, const Value & value)
{
    value.Serialize(stream, 0);
}


} // namepsace JSON
