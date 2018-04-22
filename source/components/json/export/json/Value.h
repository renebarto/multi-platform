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

    virtual bool Deserialize(std::istream & stream) = 0;
    virtual void Serialize(std::ostream & stream, int indentDepth = 0, bool indentInitial = true) const = 0;

    virtual ValueType Type() const = 0;

    virtual bool Equal(const Value & other) const = 0;

    bool operator == (const Value & other) const
    {
        return Equal(other);
    }
    bool operator != (const Value & other) const
    {
        return !Equal(other);
    }
private:
};
using ValuePtr = std::shared_ptr<Value>;

ValuePtr Parse(std::istream & stream);

inline void PrintTo(std::ostream & stream, const Value & value)
{
    value.Serialize(stream, 0);
}


} // namepsace JSON
