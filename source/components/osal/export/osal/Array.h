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
class Array
{
public:
    Array();
    explicit Array(size_t size);
    Array(const T * data, size_t length);
    Array(const Array<T> & other);
    Array(Array<T> && other);
    Array(std::initializer_list<T> data);
    virtual ~Array();
    size_t Size() const
    {
        return _size;
    }
    void Size(size_t newSize);

    const T * Data() const
    {
        return _data;
    }
    operator const T * () const
    {
        return _data;
    }
    T * Data()
    {
        return _data;
    }
    operator T * ()
    {
        return _data;
    }

    void Clear();

    T Get(size_t offset) const;
    size_t Get(size_t offset, T * data, size_t length) const;
    size_t Get(size_t offset, Array<T> & data, size_t length) const;
    Array<T> Get(size_t offset, size_t length) const;
    void Set(size_t offset, T data);
    void Set(size_t offset, const T * data, size_t length);
    void Set(size_t offset, const Array<T> & data);
    void Append(const Array<T> & data)
    {
        Set(this->Size(), data);
    }
    Array<T> & operator = (const Array<T> & other);
    Array<T> & operator = (Array<T> && other);
    bool operator == (const Array<T> & other) const;
    bool operator != (const Array<T> & other) const;
    T& operator[] (size_t offset);
    const T& operator[] (size_t offset) const;
    virtual std::ostream & PrintTo(std::ostream & stream) const;

protected:
    void AllocateSize(size_t size);
    void Free();
    void Copy(const Array<T> & other);
    void Move(Array<T> && other);

protected:
    static const size_t MinimumSize = 16;
    size_t _size;
    size_t _allocatedSize;
    T * _data;
};

template<class T>
Array<T>::Array()
    : _size(0)
    , _allocatedSize(0)
    , _data(nullptr)
{
}

template<class T>
Array<T>::Array(size_t size)
    : _size(0)
    , _allocatedSize(0)
    , _data(nullptr)
{
    AllocateSize(size);
    memset(_data, 0, size * sizeof(T));
    this->_size = size;
}

template<class T>
Array<T>::Array(const T * data, size_t size)
    : _size(0)
    , _allocatedSize(0)
    , _data(nullptr)
{
    Set(0, data, size);
}

template<class T>
Array<T>::Array(const Array<T> & other)
    : _size(0)
    , _allocatedSize(0)
    , _data(nullptr)
{
    Copy(other);
}

template<class T>
Array<T>::Array(Array<T> && other)
    : _size(0)
    , _allocatedSize(0)
    , _data(nullptr)
{
    Move(std::move(other));
}

template<class T>
Array<T>::Array(std::initializer_list<T> data)
    : _size(0)
    , _allocatedSize(0)
    , _data(nullptr)
{
    size_t offset = 0;
    for (auto const & value : data)
    {
        Set(offset++, value);
    }
}

template<class T>
Array<T>::~Array()
{
    Free();
}

template<class T>
void Array<T>::Size(size_t newSize)
{
    AllocateSize(newSize);
    if (newSize > _size)
        memset(_data + _size, 0, (newSize - _size) * sizeof(T));
    _size = newSize;
}

template<class T>
void Array<T>::Clear()
{
    _size = 0;
    memset(_data, 0, _allocatedSize * sizeof(T));
}

template<class T>
T Array<T>::Get(size_t offset) const
{
    ASSERT(offset < _size);
    return _data[offset];
}

template<class T>
size_t Array<T>::Get(size_t offset, T * data, size_t length) const
{
    ASSERT(offset < _size);
    size_t valuesToRead = std::min<size_t>(length, _size - offset);
    memcpy(data, this->_data + offset, valuesToRead * sizeof(T));
    return valuesToRead;
}

template<class T>
size_t Array<T>::Get(size_t offset, Array<T> & data, size_t length) const
{
    ASSERT(offset < _size);
    data.Clear();
    size_t valuesToRead = std::min(length, _size - offset);
    data.Set(0, this->_data + offset, valuesToRead);
    return valuesToRead;
}

template<class T>
Array<T> Array<T>::Get(size_t offset, size_t length) const
{
    ASSERT(offset + length <= _size);
    return Array<T>(Data() + offset, length);
}

template<class T>
void Array<T>::Set(size_t offset, T value)
{
    if (_allocatedSize <= offset)
    {
        AllocateSize(offset + 1);
    }
    if (_size <= offset)
    {
        _size = offset + 1;
    }
    _data[offset] = value;
}

template<class T>
void Array<T>::Set(size_t offset, const T * data, size_t length)
{
    if (_allocatedSize < length + offset)
    {
        AllocateSize(length + offset);
    }
    memcpy(this->_data + offset, data, length * sizeof(T));
    if (_size < offset + length)
    {
        this->_size = offset + length;
    }
}

template<class T>
void Array<T>::Set(size_t offset, const Array<T> & data)
{
    size_t dataSize = data.Size();
    if (_allocatedSize < dataSize + offset)
    {
        AllocateSize(dataSize + offset);
    }
    Set(offset, data.Data(), dataSize);
}

template<class T>
Array<T> & Array<T>::operator = (const Array<T> & other)
{
    if (this != &other)
    {
        Copy(other);
    }

    return *this;
}

template<class T>
Array<T> & Array<T>::operator = (Array<T> && other)
{
    if (this != &other)
    {
        Move(std::move(other));
    }

    return *this;
}

template<class T>
bool Array<T>::operator == (const Array<T> & other) const
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
bool Array<T>::operator != (const Array<T> & other) const
{
    return ! this->operator ==(other);
}

template<class T>
T& Array<T>::operator[] (size_t offset)
{
    ASSERT(offset < _size);
    return _data[offset];
}

template<class T>
const T& Array<T>::operator[] (size_t offset) const
{
    ASSERT(offset < _size);
    return _data[offset];
}

template<class T>
typename std::enable_if<!std::is_integral<T>::value, void>::type PrintTo(std::ostream & stream, T value)
{
    stream << value << " ";
}
template<class T>
typename std::enable_if<std::is_integral<T>::value, void>::type PrintTo(std::ostream & stream, T value)
{
    stream << std::hex << std::setw(2 * sizeof(T)) << std::setfill('0') << (long long)value << " ";
}
template<class T>
std::ostream & Array<T>::PrintTo(std::ostream & stream) const
{
    stream << OSAL::System::TypeName(*this)
           << " Item size: " << sizeof(T) << " Size: " << _size << " Allocated: " << _allocatedSize << std::endl;
    size_t maxValuesToDisplay = std::min(_size, MaxValuesToDisplay);
    for (size_t offset = 0; offset < maxValuesToDisplay; offset += ValuesPerRow)
    {
        for (size_t i = 0; i < ValuesPerRow; i++)
        {
            if (i + offset < maxValuesToDisplay)
            {
                T value = _data[i + offset];
                OSAL::PrintTo(stream, value);
            }
        }
        stream << std::endl;
    }
    stream << std::endl << std::flush;
    return stream;
}

template<class T>
void Array<T>::AllocateSize(size_t newSize)
{
    if (newSize < 0)
        return;
    size_t newSizeAllocated = NextPowerOfTwo(newSize);
    if (newSizeAllocated < MinimumSize)
    {
        newSizeAllocated = MinimumSize;
    }
    if (_data == nullptr)
    {
        _data = (T *)malloc(newSizeAllocated * sizeof(T));
    }
    else
    {
        _data = (T *)realloc(_data, newSizeAllocated * sizeof(T));
    }
    _allocatedSize = (_data != nullptr) ? newSizeAllocated : 0;
    if (_size > _allocatedSize)
    {
        _size = _allocatedSize;
    }
}

template<class T>
void Array<T>::Free()
{
    if (_data != nullptr)
    {
        free(_data);
    }
    _size = 0;
    _allocatedSize = 0;
}

template<class T>
void Array<T>::Copy(const Array<T> & other)
{
    Set(0, other.Data(), other.Size());
}

template<class T>
void Array<T>::Move(Array<T> && other)
{
    _size = other._size;
    _allocatedSize = other._allocatedSize;
    _data = other._data;
    other._size = 0;
    other._allocatedSize = 0;
    other._data = nullptr;
}

template<class T>
void PrintTo(const Array<T> & value, std::ostream & stream)
{
    value.PrintTo(stream);
}

template<class T>
std::ostream & operator << (std::ostream & stream, const Array<T> & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namespace OSAL
