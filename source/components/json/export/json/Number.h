#pragma once

#include "json/Value.h"

namespace JSON {

class Number : public Value
{
public:
    Number();
    Number(const std::string & value);
    Number(int8_t value);
    Number(uint8_t value);
    Number(int16_t value);
    Number(uint16_t value);
    Number(int32_t value);
    Number(uint32_t value);
    Number(int64_t value);
    Number(uint64_t value);
    Number(float value);
    Number(double value);
    Number(long double value);
    virtual ~Number() {};

    virtual bool Deserialize(std::basic_istream<char> & stream) override;
    virtual void Serialize(std::basic_ostream<char> & stream, int indentDepth = 0, bool indentInitial = true) const override;

    virtual ValueType Type() const override { return ValueType::Number; }
    const std::string & GetValue() const { return _value; }
    bool GetValue(int8_t & value) const;
    bool GetValue(uint8_t & value) const;
    bool GetValue(int16_t & value) const;
    bool GetValue(uint16_t & value) const;
    bool GetValue(int32_t & value) const;
    bool GetValue(uint32_t & value) const;
    bool GetValue(int64_t & value) const;
    bool GetValue(uint64_t & value) const;
    bool GetValue(float & value) const;
    bool GetValue(double & value) const;
    bool GetValue(long double & value) const;
    void SetValue(const std::string & value) { _value = value; }
    void SetValue(int8_t value);
    void SetValue(uint8_t value);
    void SetValue(int16_t value);
    void SetValue(uint16_t value);
    void SetValue(int32_t value);
    void SetValue(uint32_t value);
    void SetValue(int64_t value);
    void SetValue(uint64_t value);
    void SetValue(float value);
    void SetValue(double value);
    void SetValue(long double value);
    virtual void Set(ValuePtr other) override
    {
        if (other->Type() == ValueType::Number)
            SetValue(std::dynamic_pointer_cast<Number>(other)->GetValue());
    }
    virtual std::shared_ptr<Value> Clone() const
    {
        return std::make_shared<Number>(GetValue());
    }

    bool EqualTo(std::shared_ptr<Value> other) const override;

private:
    std::string _value;
};

} // namepsace JSON
