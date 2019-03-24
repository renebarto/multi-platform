#pragma once

#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <mutex>

#include "osal/osal.h"
#include "osal/Assert.h"
#include "osal/Exception.h"
#include "osal/System.h"
#include "osal/Util.h"

namespace OSAL
{

static const size_t ValuesPerRow = 8;
static const size_t MaxValuesToDisplay = 128;

template<class T>
class FixedMatrix
{
public:
    FixedMatrix() = delete;

    FixedMatrix(size_t rows, size_t cols);
    FixedMatrix(size_t rows, size_t cols, const T * data, size_t dataSize);
    FixedMatrix(size_t rows, size_t cols, const T ** data, size_t dataRows, size_t dataCols);
    FixedMatrix(size_t rows, size_t cols, T ** data, size_t dataRows, size_t dataCols);
    FixedMatrix(const FixedMatrix<T> & other);
    FixedMatrix(FixedMatrix<T> && other);

    FixedMatrix(typename std::vector< std::initializer_list<T> > data);
    virtual ~FixedMatrix();
    size_t Rows() const { return _rows; }
    size_t Cols() const { return _cols; }

    T const * const * Data() const { return _data; }
    operator T const * const * () const { return _data; }
    T ** Data() { return _data; }
    operator T ** () { return _data; }

    void Clear();

    T Get(size_t offsetRows, size_t offsetCols) const;
    size_t Get(size_t offsetRows, size_t offsetCols, T * data, size_t dataSize) const;
    void Get(size_t offsetRows, size_t offsetCols, FixedMatrix<T> & data, size_t numRows, size_t numCols) const;
    FixedMatrix<T> Get(size_t offsetRows, size_t offsetCols, size_t numRows, size_t numCols) const;
    void Set(size_t offsetRows, size_t offsetCols, T data);
    void Set(size_t offsetRows, size_t offsetCols, const T * data, size_t dataSize);
    void Set(size_t offsetRows, size_t offsetCols, const T ** data, size_t dataRows, size_t dataCols);
    void Set(size_t offsetRows, size_t offsetCols, T ** data, size_t dataRows, size_t dataCols);
    void Set(size_t offsetRows, size_t offsetCols, const FixedMatrix<T> & data);

    FixedMatrix<T> & operator = (const FixedMatrix<T> & other);
    FixedMatrix<T> & operator = (FixedMatrix<T> && other);
    bool operator == (const FixedMatrix<T> & other) const;
    bool operator != (const FixedMatrix<T> & other) const;
    T& operator() (size_t offsetRows, size_t offsetCols);
    const T& operator() (size_t offsetRows, size_t offsetCols) const;
    std::string ToString() const;

protected:
    void AllocateSize(size_t newRows, size_t newCols);
    void Free();

    size_t _rows;
    size_t _cols;
    T ** _data;

    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;
    Mutex _mutex;
};

template<class T>
FixedMatrix<T>::FixedMatrix(size_t rows, size_t cols)
    : _rows(0)
    , _cols(0)
    , _data(nullptr)
{
    AllocateSize(rows, cols);
    Clear();
}

template<class T>
FixedMatrix<T>::FixedMatrix(size_t rows, size_t cols, const T * data, size_t dataSize)
    : _rows(0)
    , _cols(0)
    , _data(nullptr)
{
    if ((rows < 0) || (cols < 0))
        return;
    AllocateSize(rows, cols);
    Clear();
    try
    {
        Set(0, 0, data, dataSize);
    }
    catch (...)
    {
        Free();
        throw;
    }
}

template<class T>
FixedMatrix<T>::FixedMatrix(size_t rows, size_t cols, const T ** data, size_t dataRows, size_t dataCols)
    : _rows(0)
    , _cols(0)
    , _data(nullptr)
{
    if ((rows < 0) || (cols < 0))
        return;
    AllocateSize(rows, cols);
    Clear();
    try
    {
        Set(0, 0, data, dataRows, dataCols);
    }
    catch (...)
    {
        Free();
        throw;
    }
}

template<class T>
FixedMatrix<T>::FixedMatrix(size_t rows, size_t cols, T ** data, size_t dataRows, size_t dataCols)
    : _rows(0)
    , _cols(0)
    , _data(nullptr)
{
    if ((rows < 0) || (cols < 0))
        return;
    AllocateSize(rows, cols);
    Clear();
    Set(0, 0, data, dataRows, dataCols);
}

template<class T>
FixedMatrix<T>::FixedMatrix(const FixedMatrix<T> & other)
    : _rows(0)
    , _cols(0)
    , _data(nullptr)
{
    AllocateSize(other.Rows(), other.Cols());
    Clear();
    Set(0, 0, other);
}

template<class T>
FixedMatrix<T>::FixedMatrix(FixedMatrix<T> && other)
    : _rows(other._rows)
    , _cols(other._cols)
    , _data(other._data)
{
    other._rows = 0;
    other._cols = 0;
    other._data = nullptr;
}

template<class T>
FixedMatrix<T>::FixedMatrix(typename std::vector<std::initializer_list<T>> data)
    : _rows(0)
    , _cols(0)
    , _data(nullptr)
{
    size_t maxWidth = 0;
    for (auto row : data)
    {
        maxWidth = max(maxWidth, row.size());
    }
    AllocateSize(data.size(), maxWidth);
    Clear();
    size_t offsetRows = 0;
    for (auto row : data)
    {
        size_t offsetCols = 0;
        for (auto value : row)
        {
            Set(offsetRows, offsetCols++, value);
        }
        offsetRows++;
    }
}

template<class T>
FixedMatrix<T>::~FixedMatrix()
{
    Free();
}

template<class T>
void FixedMatrix<T>::Clear()
{
    if (_data == nullptr)
        return;
    for (size_t x = 0; x < _rows; x++)
    {
        if (_data[x] != nullptr)
            memset(_data[x], 0, _cols * sizeof(T));
    }
}

template<class T>
T FixedMatrix<T>::Get(size_t offsetRows, size_t offsetCols) const
{
    if ((offsetRows >= 0) && (offsetRows < _rows) &&
        (offsetCols >= 0) && (offsetCols < _cols))
    {
        return _data[offsetRows][offsetCols];
    }
    throw OSAL::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
}

template<class T>
size_t FixedMatrix<T>::Get(size_t offsetRows, size_t offsetCols, T * data, size_t dataSize) const
{
    if ((offsetRows < 0) || (offsetCols < 0) || (dataSize < 0))
        throw OSAL::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
    size_t srcIndex = 0;
    size_t x = offsetRows;
    size_t y = offsetCols;
    size_t count = std::min(dataSize, (_rows - offsetRows) * (_cols - offsetCols));
    while (count > 0)
    {
        size_t valuesToCopy = std::min(_cols - y, count);
        if (valuesToCopy > 0)
        {
            memcpy(data + srcIndex, this->_data[x] + y, valuesToCopy * sizeof(T));
        }
        srcIndex += valuesToCopy;
        count -= valuesToCopy;
        y = 0;
        x++;
    }
    return dataSize;
}

template<class T>
void FixedMatrix<T>::Get(size_t offsetRows, size_t offsetCols, FixedMatrix<T> & data, size_t numRows, size_t numCols) const
{
    if ((offsetRows < 0) || (numRows < 0) || (offsetCols < 0) || (numCols < 0))
        throw OSAL::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
    size_t dataRows = std::min(numRows, _rows - offsetRows);
    size_t dataCols = std::min(numCols, _cols - offsetCols);
    data.AllocateSize(dataRows, dataCols);
    for (size_t x = 0; x < dataRows; x++)
    {
        for (size_t y = 0; y < dataCols; y++)
        {
            memcpy(data._data[x], this->_data[x + offsetRows] + offsetCols, dataCols * sizeof(T));
        }
    }
}

template<class T>
FixedMatrix<T> FixedMatrix<T>::Get(size_t offsetRows, size_t offsetCols, size_t numRows, size_t numCols) const
{
    if ((offsetRows < 0) || (numRows < 0) || (offsetCols < 0) || (numCols < 0))
        throw OSAL::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
    size_t dataRows = std::min(numRows, _rows - offsetRows);
    size_t dataCols = std::min(numCols, _cols - offsetCols);
    FixedMatrix<T> result(dataRows, dataCols);
    for (size_t x = 0; x < dataRows; x++)
    {
        for (size_t y = 0; y < dataCols; y++)
        {
            memcpy(result._data[x], this->_data[x + offsetRows] + offsetCols, dataCols * sizeof(T));
        }
    }
    return result;
}

template<class T>
void FixedMatrix<T>::Set(size_t offsetRows, size_t offsetCols, T value)
{
    if ((offsetRows < 0) || (offsetCols < 0) || (offsetRows >= _rows) || (offsetCols >= _cols))
        throw OSAL::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index / offset");
    _data[offsetRows][offsetCols] = value;
}

template<class T>
void FixedMatrix<T>::Set(size_t offsetRows, size_t offsetCols, const T * data, size_t dataSize)
{
    if ((offsetRows < 0) || (offsetCols < 0) || (dataSize < 0) || (_rows * _cols < (dataSize + offsetRows * _cols + offsetCols)))
        throw OSAL::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index / offset");
    size_t srcIndex = 0;
    size_t x = offsetRows;
    size_t y = offsetCols;
    size_t count = dataSize;
    while (count > 0)
    {
        size_t valuesToCopy = std::min(_cols - y, count);
        if (valuesToCopy > 0)
        {
            memcpy(this->_data[x] + y, data + srcIndex, valuesToCopy * sizeof(T));
        }
        srcIndex += valuesToCopy;
        count -= valuesToCopy;
        y = 0;
        x++;
    }
}

template<class T>
void FixedMatrix<T>::Set(size_t offsetRows, size_t offsetCols, const T ** data, size_t dataRows, size_t dataCols)
{
    if ((offsetRows < 0) || (offsetCols < 0) || (dataRows < 0) || (dataCols < 0) ||
        (_rows < (dataRows + offsetRows)) || (_cols < (dataCols + offsetCols)))
        throw OSAL::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index / offset");

    for (size_t x = 0; x < dataRows; x++)
    {
        for (size_t y = 0; y < dataCols; y++)
        {
            memcpy(this->_data[x + offsetRows] + offsetCols, data[x], dataCols * sizeof(T));
        }
    }
}

template<class T>
void FixedMatrix<T>::Set(size_t offsetRows, size_t offsetCols, T ** data, size_t dataRows, size_t dataCols)
{
    Set(offsetRows, offsetCols, const_cast<const T **>(data), dataRows, dataCols);
}

template<class T>
void FixedMatrix<T>::Set(size_t offsetRows, size_t offsetCols, const FixedMatrix<T> & data)
{
    if (((offsetRows < 0) || (Rows() < (data.Rows() + offsetRows))) ||
        ((offsetCols < 0) || (Cols() < (data.Cols() + offsetCols))))
        throw OSAL::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index / offset");

    for (size_t x = 0; x < data._rows; x++)
    {
        memcpy(this->_data[x + offsetRows] + offsetCols, data._data[x], data._cols * sizeof(T));
    }
}

template<class T>
FixedMatrix<T> & FixedMatrix<T>::operator = (const FixedMatrix<T> & other)
{
    if (this != &other)
    {
        Free();
        AllocateSize(other.Rows(), other.Cols());
        Set(0, 0, other);
    }

    return *this;
}

template<class T>
FixedMatrix<T> & FixedMatrix<T>::operator = (FixedMatrix<T> && other)
{
    if (this != &other)
    {
        Free();
        _rows = other._rows;
        _cols = other._cols;
        _data = other._data;
        other._rows = 0;
        other._cols = 0;
        other._data = nullptr;
    }

    return *this;
}

template<class T>
bool FixedMatrix<T>::operator == (const FixedMatrix<T> & other) const
{
    if (&other == this)
        return true;
    if ((other._rows != _rows) || (other._cols != _cols))
        return false;
    for (size_t x = 0; x < _rows; x++)
    {
        for (size_t y = 0; y < _cols; y++)
        {
            if (other._data[x][y] != _data[x][y])
                return false;
        }
    }
    return true;
}

template<class T>
bool FixedMatrix<T>::operator != (const FixedMatrix<T> & other) const
{
    return !this->operator ==(other);
}

template<class T>
T& FixedMatrix<T>::operator() (size_t offsetRows, size_t offsetCols)
{
    if ((offsetRows >= 0) && (offsetRows < _rows) && (offsetCols >= 0) && (offsetCols < _cols))
    {
        return _data[offsetRows][offsetCols];
    }
    throw OSAL::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
}

template<class T>
const T& FixedMatrix<T>::operator() (size_t offsetRows, size_t offsetCols) const
{
    if ((offsetRows >= 0) && (offsetRows < _rows) && (offsetCols >= 0) && (offsetCols < _cols))
    {
        return _data[offsetRows][offsetCols];
    }
    throw OSAL::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
}

template<class T>
std::string FixedMatrix<T>::ToString() const
{
    std::ostringstream stream;
    stream << OSAL::System::TypeName(*this) << " Item size: " << sizeof(T) << " Size X: " << _rows << ", Y" << _cols;
    return stream.str();
}

template<class T>
void FixedMatrix<T>::AllocateSize(size_t newRows, size_t newCols)
{
    Lock lock(_mutex);
    if ((newRows < 0) || (newCols < 0))
        return;
    Free();
    _data = (T **)malloc(newRows * sizeof(T *));
    for (size_t x = 0; x < newRows; x++)
    {
        _data[x] = (T*)malloc(newCols * sizeof(T));
    }
    _rows = newRows;
    _cols = newCols;
}

template<class T>
void FixedMatrix<T>::Free()
{
    Lock lock(_mutex);
    if (_data != nullptr)
    {
        for (size_t x = 0; x < _rows; x++)
        {
            if (_data[x] != nullptr)
            {
                free(_data[x]);
            }
        }
        free(_data);
    }
    _data = nullptr;
    _cols = 0;
    _rows = 0;
}

} // namespace OSAL
