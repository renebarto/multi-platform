#pragma once

#include <cassert>
#include <initializer_list>
#include <iostream>
#include "osal/OSAL.h"
WARNING_PUSH
WARNING_DISABLE(4265)
#include <mutex>
WARNING_POP
#include "core/Core.h"

namespace Core
{

template<class T>
class FixedArray
{
public:
    FixedArray(size_t size);
    FixedArray(const T * data, size_t length);
    FixedArray(const FixedArray<T> & other);
    FixedArray(FixedArray<T> && other);
    FixedArray(std::initializer_list<T> data);
    virtual ~FixedArray();
    size_t Size() const { return _size; }

    T const * Data() const { return _data; }
    operator T const * () const { return _data; }
    T * Data() { return _data; }
    operator T * () { return _data; }

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
    OSAL::String ToString() const;

protected:
    FixedArray();
    void AllocateSize(size_t size);
    void Free();

    size_t _size;
    T * _data;
    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;
    Mutex mutex;
};

template<class T>
FixedArray<T>::FixedArray(size_t size) :
    _size(0),
    _data(nullptr)
{
    AllocateSize(size);
    this->_size = size;
    Clear();
}

template<class T>
FixedArray<T>::FixedArray(const T * data, size_t size) :
    _size(0),
    _data(nullptr)
{
    if (size < 0)
        return;
    AllocateSize(size);
    Set(0, data, size);
}

template<class T>
FixedArray<T>::FixedArray(const FixedArray<T> & other) :
    _size(),
    _data(nullptr)
{
    AllocateSize(other.Size());
    Set(0, other.Data(), other.Size());
}

template<class T>
FixedArray<T>::FixedArray(FixedArray<T> && other) :
    _size(other._size),
    _data(other._data)
{
    other._size = 0;
    other._data = nullptr;
}

template<class T>
FixedArray<T>::FixedArray(std::initializer_list<T> data) :
    _size(0),
    _data(nullptr)
{
    AllocateSize(data.size());
    size_t offset = 0;
    for (auto value : data)
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
    bzero(_data, _size * sizeof(T));
}

template<class T>
T FixedArray<T>::Get(size_t offset) const
{
    assert((offset >= 0) && (offset < _size));
    return _data[offset];
}

template<class T>
size_t FixedArray<T>::Get(size_t offset, T * data, size_t length) const
{
    assert((offset >= 0) && (length >= 0));
    size_t valuesToRead = std::min(length, _size - offset);
    T * dataPtr = data;
    size_t currentOffset = offset;
    for (size_t i = 0; i < valuesToRead; i++)
    {
        *dataPtr++ = Get(currentOffset++);
    }
    return valuesToRead;
}

template<class T>
size_t FixedArray<T>::Get(size_t offset, FixedArray<T> & data, size_t length) const
{
    assert((offset >= 0) && (length >= 0));
    size_t valuesToRead = std::min(length, _size - offset);
    data.AllocateSize(valuesToRead);
    size_t currentOffset = offset;
    for (size_t i = 0; i < valuesToRead; i++)
    {
        data.Set(i, Get(currentOffset++));
    }
    return valuesToRead;
}

template<class T>
FixedArray<T> FixedArray<T>::Get(size_t offset, size_t length) const
{
    assert((offset >= 0) && (length >= 0));
    return FixedArray<T>(Data() + offset, length);
}

template<class T>
void FixedArray<T>::Set(size_t offset, T value)
{
    assert((offset >= 0) && (offset < _size));
    _data[offset] = value;
}

template<class T>
void FixedArray<T>::Set(size_t offset, const T * data, size_t length)
{
    assert((offset >= 0) && (length >= 0) && (offset + length <= _size));
    memcpy(this->_data + offset, data, length * sizeof(T));
}

template<class T>
void FixedArray<T>::Set(size_t offset, const FixedArray<T> & data)
{
    size_t dataSize = data.Size();
    assert((offset >= 0) && (offset + dataSize <= _size));
    memcpy(this->_data + offset, data.Data(), dataSize * sizeof(T));
}

template<class T>
FixedArray<T> & FixedArray<T>::operator = (const FixedArray<T> & other)
{
    if (this != &other)
    {
        AllocateSize(other.Size());
        Set(0, other.Data(), other.Size());
    }

    return *this;
}

template<class T>
FixedArray<T> & FixedArray<T>::operator = (FixedArray<T> && other)
{
    if (this != &other)
    {
        _size = other._size;
        _data = other._data;
        other._size = 0;
        other._data = nullptr;
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
    assert((offset >= 0) && (offset < _size));
    return _data[offset];
}

template<class T>
const T& FixedArray<T>::operator[] (size_t offset) const
{
    assert((offset >= 0) && (offset < _size));
    return _data[offset];
}

template<class T>
std::string FixedArray<T>::ToString() const
{
    std::ostringstream stream;
    stream << OSAL::OS::TypeName(*this) << " Item size: " << sizeof(T) << " Size: " << _size;
    return stream.str();
}

template<class T>
void FixedArray<T>::AllocateSize(size_t newSize)
{
    if (newSize < 0)
        return;

    Lock lock(mutex);
    Free();
    _data = (T *)malloc(newSize * sizeof(T));
    _size = newSize;
}

template<class T>
void FixedArray<T>::Free()
{
    Lock lock(mutex);
    if (_data != nullptr)
    {
        free(_data);
    }
    _data = nullptr;
    _size = 0;
}

template<class T>
inline void PrintTo(const FixedArray<T> & value, std::ostream & stream)
{
    stream << value.ToString();
}

} // namespace Core
