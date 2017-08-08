#pragma once

#include "json/Value.h"

namespace JSON {

class String : public Value
{
public:
    String();
    String(const OSAL::String & value);

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) override;

    virtual ValueType Type() const override { return ValueType::String; }
    const OSAL::String & GetValue() const { return _value; }
    void SetValue(const OSAL::String & value) { _value = value; }

private:
    OSAL::String _value;
};

} // namepsace JSON
