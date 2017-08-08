#pragma once

#include "json/Value.h"

namespace JSON {

class KVPair
{
public:
    KVPair();
    KVPair(const OSAL::String & key, const ValuePtr & value);

    const OSAL::String & GetKey() const { return _key; }
    const ValuePtr GetValue() const { return _value; }
    bool Deserialize(std::basic_istream<OSAL::Char> & stream);
    void Serialize(std::basic_ostream<OSAL::Char> & stream);

private:
    OSAL::String _key;
    ValuePtr _value;
};

class Object : public Value
{
public:
    Object();

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) override;

    virtual ValueType Type() const override { return ValueType::Object; }

private:
};

class Array : public Value
{
public:
    Array();

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) override;

    virtual ValueType Type() const override { return ValueType::Array; }

private:
};

} // namepsace JSON
