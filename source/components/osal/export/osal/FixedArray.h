#pragma once

#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <iomanip>
#include <iostream>

#include "osal/osal.h"
#include "osal/Assert.h"
#include "osal/System.h"
#include "osal/Util.h"

namespace OSAL
{

static const size_t ValuesPerRow = 8;
static const size_t MaxValuesToDisplay = 128;

template<class T>
class FixedArray
{
public:
    FixedArray() = delete;
    explicit FixedArray(size_t size);
    FixedArray(const T * data, size_t length);
    FixedArray(const FixedArray<T> & other);
    FixedArray(FixedArray<T> && other);
    FixedArray(std::initializer_list<T> data);
    virtual ~FixedArray();
    size_t Size() const
    {
        return _size;
    }

    const T * Data() const
    {
        return _data;
    }
    T * Data()
    {
        return _data;
    }

    void Clear();

    T Get(size_t offset) const;
    size_t Get(size_t offset, T * data, size_t length) const;
    size_t Get(size_t offset, FixedArray<T> & data, size_t length) const;
    FixedArray<T> Get(size_t offset, size_t length) const;
    void Set(size_t offset, T data);
    void Set(size_t offset, const T * data, size_t length);
    void Set(size_t offset, const FixedArray<T> & data);
    FixedArray<T> & operator = (const FixedArray<T> & other);
    FixedArray<T> & operator = (FixedArray<T> && other);
    bool operator == (const FixedArray<T> & other) const;
    bool operator != (const FixedArray<T> & other) const;
    T& operator[] (size_t offset);
    const T& operator[] (size_t offset) const;
    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const;

protected:
    void AllocateSize(size_t size);
    void Free();
    void Copy(const FixedArray<T> & other);
    void Move(FixedArray<T> && other);

protected:
    size_t _size;
    T * _data;
};

template<class T>
FixedArray<T>::FixedArray(size_t size)
    : _size(0)
    , _data(nullptr)
{
    AllocateSize(size);
    Clear();
}

template<class T>
FixedArray<T>::FixedArray(const T * data, size_t size)
    : _size(0)
    , _data(nullptr)
{
    AllocateSize(size);
    Clear();
    Set(0, data, size);
}

template<class T>
FixedArray<T>::FixedArray(const FixedArray<T> & other)
    : _size(0)
    , _data(nullptr)
{
    Copy(other);
}

template<class T>
FixedArray<T>::FixedArray(FixedArray<T> && other)
    : _size(0)
    , _data(nullptr)
{
    Move(std::move(other));
}

template<class T>
FixedArray<T>::FixedArray(std::initializer_list<T> data)
    : _size(0)
    , _data(nullptr)
{
    AllocateSize(data.size());
    Clear();
    size_t offset = 0;
    for (auto const & value : data)
    {
        Set(offset++, value);
    }
}

template<class T>
FixedArray<T>::~FixedArray()
{
    Free();
}

template<class T>
void FixedArray<T>::Clear()
{
    if (_data == nullptr)
        return;
    memset(_data, 0, _size * sizeof(T));
}

template<class T>
T FixedArray<T>::Get(size_t offset) const
{
    ASSERT(offset < _size);
    if (offset >= _size)
        return 0;
    return _data[offset];
}

template<class T>
size_t FixedArray<T>::Get(size_t offset, T * data, size_t length) const
{
    ASSERT(offset < _size);
    if (offset >= _size)
        return 0;
    size_t valuesToRead = std::min<size_t>(length, _size - offset);
    memcpy(data, this->_data + offset, valuesToRead * sizeof(T));
    return valuesToRead;
}

template<class T>
size_t FixedArray<T>::Get(size_t offset, FixedArray<T> & data, size_t length) const
{
    ASSERT(offset < _size);
    data.Clear();
    if (offset >= _size)
        return 0;
    size_t valuesToRead = std::min(length, _size - offset);
    data.Set(0, this->_data + offset, valuesToRead);
    return valuesToRead;
}

template<class T>
FixedArray<T> FixedArray<T>::Get(size_t offset, size_t length) const
{
    ASSERT(offset + length <= _size);
    if (offset + length > _size)
        return FixedArray<T>(0);
    return FixedArray<T>(Data() + offset, length);
}

template<class T>
void FixedArray<T>::Set(size_t offset, T value)
{
    ASSERT(_size > offset);
    _data[offset] = value;
}

template<class T>
void FixedArray<T>::Set(size_t offset, const T * data, size_t length)
{
    ASSERT(_size >= length + offset);
    memcpy(this->_data + offset, data, length * sizeof(T));
}

template<class T>
void FixedArray<T>::Set(size_t offset, const FixedArray<T> & data)
{
    size_t dataSize = data.Size();
    ASSERT(_size >= dataSize + offset);
    Set(offset, data.Data(), dataSize);
}

template<class T>
FixedArray<T> & FixedArray<T>::operator = (const FixedArray<T> & other)
{
    if (this != &other)
    {
        Copy(other);
    }

    return *this;
}

template<class T>
FixedArray<T> & FixedArray<T>::operator = (FixedArray<T> && other)
{
    if (this != &other)
    {
        Move(std::move(other));
    }

    return *this;
}

template<class T>
bool FixedArray<T>::operator == (const FixedArray<T> & other) const
{
    if (&other == this)
        return true;
    if (other._size != _size)
        return false;
    for (size_t i = 0; i < _size; i++)
    {
        if (other._data[i] != _data[i])
            return false;
    }
    return true;
}

template<class T>
bool FixedArray<T>::operator != (const FixedArray<T> & other) const
{
    return ! this->operator ==(other);
}

template<class T>
T& FixedArray<T>::operator[] (size_t offset)
{
    ASSERT(offset < _size);
    if (offset >= _size)
        return _data[0];
    return _data[offset];
}

template<class T>
const T& FixedArray<T>::operator[] (size_t offset) const
{
    ASSERT(offset < _size);
    if (offset >= _size)
        return _data[0];
    return _data[offset];
}

template<class T>
template<class Elem, class Traits>
std::basic_ostream<Elem, Traits> & FixedArray<T>::PrintTo(std::basic_ostream<Elem, Traits> & s) const
{
    s << OSAL::System::TypeName(*this)
           << " Item size: " << sizeof(T) << " Size: " << _size << std::endl;
    size_t maxValuesToDisplay = std::min(_size, MaxValuesToDisplay);
    for (size_t offset = 0; offset < maxValuesToDisplay; offset += ValuesPerRow)
    {
        for (size_t i = 0; i < ValuesPerRow; i++)
        {
            if (i + offset < maxValuesToDisplay)
            {
                T value = _data[i + offset];
                if (std::is_integral<T>::value)
                    s << std::hex << std::setw(2 * sizeof(T)) << std::setfill('0') << (long long)value << " ";
                else
                    s << value << " ";
            }
        }
        s << std::endl;
    }
    s << std::endl << std::flush;
    return s;
}

template<class T>
void FixedArray<T>::AllocateSize(size_t newSize)
{
    if (newSize < 0)
        return;
    Free();
    _data = reinterpret_cast<T *>(malloc(newSize * sizeof(T)));
    _size = newSize;
}

template<class T>
void FixedArray<T>::Free()
{
    if (_data != nullptr)
    {
        free(_data);
    }
    _size = 0;
}

template<class T>
void FixedArray<T>::Copy(const FixedArray<T> & other)
{
    AllocateSize(other.Size());
    Set(0, other.Data(), other.Size());
}

template<class T>
void FixedArray<T>::Move(FixedArray<T> && other)
{
    Free();
    _size = other._size;
    _data = other._data;
    other._size = 0;
    other._data = nullptr;
}

template <class Elem, class Traits, class T>
std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s, const FixedArray<T> & value)
{
    return value.PrintTo(s);
}

template <class Elem, class Traits, class T>
std::basic_ostream<Elem, Traits> & operator<<(std::basic_ostream<Elem, Traits> &s, const FixedArray<T> & value)
{
    return value.PrintTo(s);
}

} // namespace OSAL
