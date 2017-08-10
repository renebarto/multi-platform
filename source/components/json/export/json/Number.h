#pragma once

#include "json/Value.h"

namespace JSON {

class Number : public Value
{
public:
    Number();
    Number(const OSAL::String & value);
    virtual ~Number() {};

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream, int indentDepth = 0) const override;

    virtual ValueType Type() const override { return ValueType::Number; }
    const OSAL::String & GetValue() const { return _value; }
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
    void SetValue(const OSAL::String & value) { _value = value; }
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

private:
    OSAL::String _value;
};

} // namepsace JSON
