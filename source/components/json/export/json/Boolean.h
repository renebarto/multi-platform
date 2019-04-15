#pragma once

#include "json/Value.h"

namespace JSON {

class Boolean : public Value
{
public:
    Boolean();
    Boolean(bool value);
    virtual ~Boolean() {};

    virtual bool Deserialize(std::basic_istream<char> & stream) override;
    virtual void Serialize(std::basic_ostream<char> & stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual ValueType Type() const override { return ValueType::Boolean; }
    virtual void Set(ValuePtr other) override
    {
        if (other->Type() == ValueType::Boolean)
            SetValue(std::dynamic_pointer_cast<Boolean>(other)->GetValue());
    }
    virtual std::shared_ptr<Value> Clone() const
    {
        return std::make_shared<Boolean>(GetValue());
    }

    bool GetValue() const { return _value; }
    void SetValue(bool value) { _value = value; }
    bool EqualTo(std::shared_ptr<Value> other) const override;

private:
    bool _value;
};

} // namepsace JSON
