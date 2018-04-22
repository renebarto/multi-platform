#pragma once

#include "json/Value.h"

namespace JSON {

class String : public Value
{
public:
    String();
    String(const std::string & value);
    virtual ~String() {};

    virtual bool Deserialize(std::istream & stream) override;
    virtual void Serialize(std::ostream & stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual ValueType Type() const override { return ValueType::String; }
    const std::string & GetValue() const { return _value; }
    void SetValue(const std::string & value) { _value = value; }

    bool Equal(const Value & other) const override
    {
        if (Type() != other.Type())
            return false;
        return (GetValue() == dynamic_cast<const String &>(other).GetValue());
    }

private:
    std::string _value;
};

} // namepsace JSON
