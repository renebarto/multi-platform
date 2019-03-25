#pragma once

#include <osal/Unused.h>
#include "json/Value.h"

namespace JSON {

class Null : public Value
{
public:
    Null();
    virtual ~Null() {};

    virtual bool Deserialize(std::basic_istream<char> & stream) override;
    virtual void Serialize(std::basic_ostream<char> & stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual ValueType Type() const override { return ValueType::Null; }
    virtual void Set(ValuePtr UNUSED(other)) override
    {
    }
    virtual std::shared_ptr<Value> Clone() const
    {
        return std::make_shared<Null>();
    }

    bool EqualTo(std::shared_ptr<Value> other) const override;

private:
};

} // namepsace JSON
