#pragma once

#include <osal/Assert.h>

namespace Core
{

template <class T>
class nullable
{
public:
    static const nullable<T> Null;

    nullable()
        : _hasValue(false)
        , _value()
    {}
    nullable(const nullable & other)
        : _hasValue(other._hasValue)
        , _value(other._value)
    {}
    nullable(nullable && other)
        : _hasValue(other._hasValue)
        , _value(std::move(other._value))
    {
        other._hasValue = false;
    }
    nullable(const T & _value)
        : _hasValue(true)
        , _value(_value)
    {}
    virtual ~nullable() {}

    nullable & operator =(const T & _value)
    {
        this->_hasValue = true;
        this->_value = _value;
        return *this;
    }
    nullable & operator =(const nullable & other)
    {
        if (this != &other)
        {
            this->_hasValue = other._hasValue;
            this->_value = other._value;
        }
        return *this;
    }
    nullable & operator =(nullable && other)
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
        ASSERT(_hasValue);
        return _value;
    }
    const T & GetValue() const
    {
        ASSERT(_hasValue);
        return _value;
    }
};

template <class T>
const nullable<T> nullable<T>::Null = nullable<T>();

} // namespace Core
