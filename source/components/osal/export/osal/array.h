#pragma once

#include <algorithm>
#include <initializer_list>
#include <vector>

#include "osal/osal.h"
#include "osal/Assert.h"
#include "osal/System.h"

namespace OSAL
{

template<class T>
class array
{
public:
    static const size_t ValuesPerRow = 8;
    static const size_t MaxValuesToDisplay = 128;

    constexpr array();
    explicit array(size_t size);
    array(const std::initializer_list<T> & data);
    array(const T * data, size_t length);
    array(const array<T> & other);
    array(array<T> && other);
    array<T> & operator = (const array<T> & other);
    array<T> & operator = (array<T> && other);

    constexpr size_t size() const
    {
        return _data.size();
    }
    void resize(size_t newSize);

    constexpr const T * data() const noexcept
    {
        return _data.data();
    }
    T * data() noexcept
    {
        return _data.data();
    }

    void clear();

    T& operator[] (size_t offset);
    const T& operator[] (size_t offset) const;

    T get(size_t offset) const;
    size_t get(size_t offset, T * data, size_t length) const;
    size_t get(size_t offset, array<T> & data, size_t length) const;
    array<T> get(size_t offset, size_t length) const;
    void set(size_t offset, T data);
    void set(size_t offset, const T * data, size_t length);
    void set(size_t offset, const array<T> & data);
    void append(T data)
    {
        set(size(), data);
    }
    void append(const array<T> & data)
    {
        set(size(), data);
    }
    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const
    {
        return s << *this;
    }

    template<class Elem>
    friend bool operator == (const array<Elem> & lhs, const array<Elem> & rhs);
    template<class Elem>
    friend bool operator != (const array<Elem> & lhs, const array<Elem> & rhs);
    template <class Elem, class Traits>
    friend std::basic_ostream<Elem, Traits> & operator << (std::basic_ostream<Elem, Traits> & s, const array<T> & value);

protected:
    static constexpr size_t MinimumSize = 16;
    std::vector<T> _data;
};

template<class T>
constexpr array<T>::array()
    : _data()
{
}

template<class T>
array<T>::array(size_t size)
    : _data(size)
{
}

template<class T>
array<T>::array(const std::initializer_list<T> & data)
    : _data(data.begin(), data.end())
{
}

template<class T>
array<T>::array(const T * data, size_t size)
    : _data(&data[0], &data[size])
{
}

template<class T>
array<T>::array(const array<T> & other)
    : _data(other._data)
{
}

template<class T>
array<T>::array(array<T> && other)
    : _data(std::move(other._data))
{
}

template<class T>
array<T> & array<T>::operator = (const array<T> & other)
{
    if (this != &other)
    {
        _data = other._data;
    }

    return *this;
}

template<class T>
array<T> & array<T>::operator = (array<T> && other)
{
    if (this != &other)
    {
        _data = std::move(other._data);
    }

    return *this;
}

template<class T>
void array<T>::resize(size_t newSize)
{
    _data.resize(newSize, T{});
}

template<class T>
void array<T>::clear()
{
    _data.clear();
}

template<class T>
T& array<T>::operator[] (size_t offset)
{
    ASSERT(offset < size());
    if (offset >= size())
        return _data[0];
    return _data[offset];
}

template<class T>
const T& array<T>::operator[] (size_t offset) const
{
    ASSERT(offset < size());
    if (offset >= size())
        return _data[0];
    return _data[offset];
}

template<class T>
T array<T>::get(size_t offset) const
{
    ASSERT(offset < size());
    if (offset >= size())
        return 0;
    return _data[offset];
}

template<class T>
size_t array<T>::get(size_t offset, T * data, size_t length) const
{
    ASSERT(offset + length <= size());
    if (offset + length > size())
        return 0;
    memcpy(data, this->data() + offset, length * sizeof(T));
    return length;
}

template<class T>
size_t array<T>::get(size_t offset, array<T> & data, size_t length) const
{
    ASSERT(offset + length <= size());
    data.clear();
    if (offset + length > size())
        return 0;
    data.set(0, this->data() + offset, length);
    return length;
}

template<class T>
array<T> array<T>::get(size_t offset, size_t length) const
{
    ASSERT(offset + length <= size());
    if (offset + length > size())
        return array<T>();
    return array<T>(data() + offset, length);
}

template<class T>
void array<T>::set(size_t offset, T value)
{
    if (size() <= offset)
    {
        resize(offset + 1);
    }
    _data[offset] = value;
}

template<class T>
void array<T>::set(size_t offset, const T * data, size_t length)
{
    if (size() < length + offset)
    {
        resize(length + offset);
    }
    std::copy(data, data + length, _data.begin() + static_cast<int64_t>(offset));
}

template<class T>
void array<T>::set(size_t offset, const array<T> & data)
{
    if (size() < data.size() + offset)
    {
        resize(data.size() + offset);
    }
    set(offset, data.data(), data.size());
}

template<class T>
bool operator == (const array<T> & lhs, const array<T> & rhs)
{
    return lhs._data == rhs._data;
}

template<class T>
bool operator != (const array<T> & lhs, const array<T> & rhs)
{
    return !(lhs == rhs);
}

template <class Elem, class Traits, class T>
typename std::enable_if<!std::is_integral<Elem>::value, void>::type PrintTo(std::basic_ostream<Elem, Traits> & stream, T value)
{
    stream << value << " ";
}

template <class Elem, class Traits, class T>
typename std::enable_if<std::is_integral<Elem>::value, void>::type PrintTo(std::basic_ostream<Elem, Traits> & stream, T value)
{
    stream << std::hex << std::setw(2 * sizeof(T)) << std::setfill('0') << (long long)value << " ";
}

template <class Elem, class Traits, class T>
std::basic_ostream<Elem, Traits> & operator<<(std::basic_ostream<Elem, Traits> &s, const array<T> & value)
{
    s << OSAL::System::TypeName(value)
      << " Item size: " << sizeof(T) << " Size: " << value.size() << std::endl;
    size_t maxValuesToDisplay = std::min(value.size(), array<T>::MaxValuesToDisplay);
    for (size_t offset = 0; offset < maxValuesToDisplay; offset += array<T>::ValuesPerRow)
    {
        for (size_t i = 0; i < array<T>::ValuesPerRow; i++)
        {
            if (i + offset < maxValuesToDisplay)
            {
                T elementValue = value[i + offset];
                OSAL::PrintTo(s, elementValue);
            }
        }
        s << std::endl;
    }
    return s << std::endl << std::flush;
}

template <class Elem, class Traits, class T>
std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & stream, const array<T> & value)
{
    return value.PrintTo(stream);
}

} // namespace OSAL