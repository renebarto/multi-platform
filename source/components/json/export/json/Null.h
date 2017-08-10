#pragma once

#include "json/Value.h"

namespace JSON {

class Null : public Value
{
public:
    Null();
    virtual ~Null() {};

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual ValueType Type() const override { return ValueType::Null; }

private:
};

} // namepsace JSON
