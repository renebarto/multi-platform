#pragma once

#include <stdexcept>

namespace Core
{

template <class T>
class Nullable
{
public:
    static const Nullable<T> Null;

    Nullable()
        : _hasValue(false)
        , _value()
    {}
    Nullable(const Nullable & other)
        : _hasValue(other._hasValue)
        , _value(other._value)
    {}
    Nullable(Nullable && other)
        : _hasValue(other._hasValue)
        , _value(std::move(other._value))
    {}
    Nullable(const T & _value)
        : _hasValue(true)
        , _value(_value)
    {}
    virtual ~Nullable() {}

    Nullable & operator =(const T & _value)
    {
        this->_hasValue = true;
        this->_value = _value;
        return *this;
    }
    Nullable & operator =(const Nullable & other)
    {
        if (this != &other)
        {
            this->_hasValue = other._hasValue;
            this->_value = other._value;
        }
        return *this;
    }
    Nullable & operator =(Nullable && other)
    {
        if (this != &other)
        {
            this->_hasValue = other._hasValue;
            this->_value = std::move(other._value);
            other._hasValue = false;
        }
        return *this;
    }

    void SetNull()
    {
        _hasValue = false;
        _value = T{};
    }
    bool HasValue() const { return _hasValue; }
    T & Value() { return GetValue(); }
    const T & Value() const { return GetValue(); }

protected:
    bool _hasValue;
    T _value;

    T & GetValue()
    {
        if (!_hasValue)
            throw std::runtime_error("Nullable object has null value");
        return _value;
    }
};

template <class T>
const Nullable<T> Nullable<T>::Null = Nullable<T>();

} // namespace Core
