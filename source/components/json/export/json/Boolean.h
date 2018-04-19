#pragma once

#include "json/Value.h"

namespace JSON {

class Boolean : public Value
{
public:
    Boolean();
    Boolean(bool value);
    virtual ~Boolean() {};

    virtual bool Deserialize(std::istream & stream) override;
    virtual void Serialize(std::ostream & stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual ValueType Type() const override { return ValueType::Boolean; }
    bool GetValue() const { return _value; }
    void SetValue(bool value) { _value = value; }

private:
    bool _value;
};

} // namepsace JSON
