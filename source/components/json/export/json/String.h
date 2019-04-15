#pragma once

#include "json/Value.h"

namespace JSON {

class String : public Value
{
public:
    String();
    String(const std::string & value);
    virtual ~String() {};

    virtual bool Deserialize(std::basic_istream<char> & stream) override;
    virtual void Serialize(std::basic_ostream<char> & stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual ValueType Type() const override { return ValueType::String; }
    virtual void Set(ValuePtr other) override
    {
        if (other->Type() == ValueType::String)
            SetValue(std::dynamic_pointer_cast<String>(other)->GetValue());
    }
    virtual std::shared_ptr<Value> Clone() const
    {
        return std::make_shared<String>(GetValue());
    }
    const std::string & GetValue() const { return _value; }
    void SetValue(const std::string & value) { _value = value; }

    bool EqualTo(std::shared_ptr<Value> other) const override;

private:
    std::string _value;
};

} // namepsace JSON
