#pragma once

#include "json/Value.h"

namespace JSON {

class Null : public Value
{
public:
    Null();

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) const override;

    virtual ValueType Type() const override { return ValueType::Null; }

private:
};

} // namepsace JSON
