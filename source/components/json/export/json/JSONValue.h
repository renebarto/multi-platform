#pragma once

#include "json/exports.h"
#include <iostream>
#include <memory>
#include "osal/Strings.h"

namespace JSON {

class Array;
class Object;
enum class ValueType
{
    Null,
    Boolean,
    String,
    Number,
    Object,
    Array,
};
class Value
{
public:
    Value();

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) = 0;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) = 0;

    virtual ValueType Type() = 0;
private:
};
using ValuePtr = std::shared_ptr<Value>;

ValuePtr Parse(std::basic_istream<OSAL::Char> & stream);

class ObjectKVPair
{
public:
    ObjectKVPair();
    ObjectKVPair(const OSAL::String & key, const ValuePtr & value);

    const OSAL::String & GetKey() const { return _key; }
    const Value & GetValue() const { return *_value; }
    Value & GetValue() { return *_value; }
    bool Deserialize(std::basic_istream<OSAL::Char> & stream);
    void Serialize(std::basic_ostream<OSAL::Char> & stream);

private:
    OSAL::String _key;
    ValuePtr _value;
};

class Null : public Value
{
public:
    Null();

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) override;

    virtual ValueType Type() override { return ValueType::Null; }

private:
};

class Boolean : public Value
{
public:
    Boolean();
    Boolean(bool value);

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) override;

    virtual ValueType Type() override { return ValueType::Boolean; }
    bool GetValue() { return _value; }
    void SetValue(bool value) { _value = value; }

private:
    bool _value;
};

class Number : public Value
{
public:
    Number();

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) override;

    virtual ValueType Type() override { return ValueType::Number; }

private:
};

class String : public Value
{
public:
    String();

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) override;

    virtual ValueType Type() override { return ValueType::String; }

private:
};

class Object : public Value
{
public:
    Object();

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) override;

    virtual ValueType Type() override { return ValueType::Object; }

private:
};

class Array : public Value
{
public:
    Array();

    virtual bool Deserialize(std::basic_istream<OSAL::Char> & stream) override;
    virtual void Serialize(std::basic_ostream<OSAL::Char> & stream) override;

    virtual ValueType Type() override { return ValueType::Array; }

private:
};

} // namepsace JSON
