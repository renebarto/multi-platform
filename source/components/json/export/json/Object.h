#pragma once

#include <vector>
#include "json/Value.h"

namespace JSON {

class KVPair
{
public:
    KVPair();
    KVPair(const OSAL::String & key, const ValuePtr & value);

    const OSAL::String & GetKey() const { return _key; }
    void SetKey(const OSAL::String & value) { _key = value; }
    const ValuePtr GetValue() const { return _value; }
    void SetValue(const ValuePtr value) { _value = value; }
    bool Deserialize(std::basic_istream<OSAL::Char> & stream);
    void Serialize(std::basic_ostream<OSAL::Char> & stream) const;

private:
    OSAL::String _key;
    ValuePtr _value;
};

using KVList = std::vector<KVPair>;

class Object : public Value
{
public:
    Object();
    Object(std::initializer_list<KVPair> & elements);

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) const override;

    virtual ValueType Type() const override { return ValueType::Object; }
    const KVList & GetElements() const { return _elements; }

private:
    KVList _elements;
};

class Array : public Value
{
public:
    Array();

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) const override;

    virtual ValueType Type() const override { return ValueType::Array; }

private:
};

} // namepsace JSON
