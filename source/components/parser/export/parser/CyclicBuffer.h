#pragma once

#include <cassert>
#include <cstring>
#include <memory>
#include <sstream>
#include <vector>

namespace Parser
{

inline void CopyMemory(void * dst, size_t dstOffset, const void * src, size_t srcOffset, size_t count)
{
    memcpy(reinterpret_cast<uint8_t *>(dst) + dstOffset, reinterpret_cast<const uint8_t *>(src) + srcOffset, count);
}
    
inline void ZeroMemory(void * dst, size_t dstOffset, size_t count)
{
    memset(reinterpret_cast<uint8_t *>(dst) + dstOffset, 0, count);
}
    
template<class T>
class CyclicBuffer
{
public:
    CyclicBuffer(size_t size)
        : _buf()
        , _fill()
        , _size()
        , _start()
        , _end()
        , _offset()
    {
        SetSize(size);
    }
    CyclicBuffer(const CyclicBuffer &) = delete;

    virtual ~CyclicBuffer()
    {
        SetSize(0);
    }

    CyclicBuffer & operator = (const CyclicBuffer &) = delete;

    size_t Offset() const { return _offset; }
    size_t Size() const { return _size; }
    size_t Fill() const { return _fill; }
    size_t Start() const { return _start; }
    size_t End() const { return _end; }

    bool IsFull() const { return _fill == _size; }
    size_t Space() const { return _size - _fill; }
    bool IsEmpty() const { return _fill == 0; }

    size_t Read(T * items, size_t maxItems)
    {
        return Read(items, maxItems, 0);
    }
    size_t Read(T * items, size_t maxItems, size_t offset)
    {
        size_t result = 0;
        return TryRead(items, maxItems, offset);
    }
    bool Read(T & item)
    {
        return TryRead(item);
    }

    size_t Peek(T * items, size_t maxItems)
    {
        return Peek(items, maxItems, 0);
    }
    size_t Peek(T * items, size_t maxItems, size_t offset)
    {
        return TryPeek(items, maxItems, offset);
    }
    size_t Write(T * items, size_t itemCount)
    {
        return Write(items, itemCount, 0);
    }
    size_t Write(T * items, size_t itemCount, size_t offset)
    {
        return TryWrite(items, itemCount, offset);
    }
    bool Write(const T & item)
    {
        return TryWrite(item);
    }

    void Flush();

    virtual std::string ToString() const
    {
        std::ostringstream stream;
        stream << "#:" << _fill << "/" << _size << " S:" << _start << " E:" << _end;
        return stream.str();
    }

protected:
    void SetSize(size_t newSize);

    size_t TryRead(T * items, size_t maxItems, size_t offset);
    bool TryRead(T & item);
    size_t TryPeek(T * items, size_t maxItems, size_t offset);
    size_t TryWrite(T * items, size_t itemCount, size_t offset);
    bool TryWrite(const T & item);

    std::vector<T> _buf;
    size_t _fill;
    size_t _size;
    size_t _start;
    size_t _end;
    size_t _offset;
};

template<class T>
void CyclicBuffer<T>::SetSize(size_t newSize)
{
    _buf.reserve(newSize);
    _fill = 0;
    _size = newSize;
    _start = 0;
    _end = 0;
}

template<class T>
size_t CyclicBuffer<T>::TryRead(T * items, size_t maxItems, size_t offset)
{
    size_t itemsRead;
    size_t numItemsToDo = std::min(maxItems, _fill);
    if (numItemsToDo > 0)
    {
        size_t numItemsLastPart = std::min(Size() - _start, numItemsToDo);
        size_t numItemsFirstPart = numItemsToDo - numItemsLastPart;
        if (numItemsLastPart > 0)
        {
            CopyMemory(items, offset * sizeof(T), _buf.data(), _start * sizeof(T), numItemsLastPart * sizeof(T));
            ZeroMemory(_buf.data(), _start * sizeof(T), numItemsLastPart * sizeof(T));
            offset += numItemsLastPart;
        }
        if (numItemsFirstPart > 0)
        {
            CopyMemory(items, offset * sizeof(T), _buf.data(), 0, numItemsFirstPart * sizeof(T));
            ZeroMemory(_buf.data(), 0, numItemsFirstPart * sizeof(T));
        }
        _start = (_start + numItemsToDo) % Size();
        _offset += numItemsToDo;
        _fill -= numItemsToDo;
        itemsRead = numItemsToDo;
    }
    else
    {
        itemsRead = 0;
    }
    return itemsRead;
}

template<class T>
bool CyclicBuffer<T>::TryRead(T & item)
{
    bool result = false;
    ZeroMemory(&item, 0, sizeof(T));
    size_t numItemsToDo = std::min(size_t(1), _fill);
    if (numItemsToDo > 0)
    {
        item = _buf[_start];
        ZeroMemory(&_buf[_start], 0, sizeof(T));
        _start = (_start + 1) % Size();
        _offset++;
        _fill--;
        result = true;
    }
    return result;
}

template<class T>
size_t CyclicBuffer<T>::TryPeek(T * items, size_t maxItems, size_t offset)
{
    size_t itemsRead = 0;
    size_t numItemsToDo = std::min(maxItems, _fill);
    if (numItemsToDo > 0)
    {
        size_t numItemsLastPart = std::min(Size() - _start, numItemsToDo);
        size_t numItemsFirstPart = numItemsToDo - numItemsLastPart;
        if (numItemsLastPart > 0)
        {
            CopyMemory(items, offset * sizeof(T), _buf.data(), _start * sizeof(T), numItemsLastPart * sizeof(T));
            offset += numItemsLastPart;
        }
        if (numItemsFirstPart > 0)
        {
            CopyMemory(items, offset * sizeof(T), _buf.data(), 0, numItemsFirstPart * sizeof(T));
        }
        itemsRead = numItemsToDo;
    }
    return itemsRead;
}

template<class T>
size_t CyclicBuffer<T>::TryWrite(T * items, size_t itemCount, size_t offset)
{
    assert(itemCount <= Space());
    size_t itemsWritten;
    itemCount = std::min(itemCount, Space());
    if (itemCount > 0)
    {
        size_t numItemsLastPart = std::min(Size() - _end, itemCount);
        size_t numItemsFirstPart = itemCount - numItemsLastPart;
        if (numItemsLastPart > 0)
        {
            CopyMemory(_buf.data(), _end * sizeof(T), items, 0, numItemsLastPart * sizeof(T));
            offset += numItemsLastPart;
        }
        if (numItemsFirstPart > 0)
        {
            CopyMemory(_buf.data(), 0, items, offset * sizeof(T), numItemsFirstPart * sizeof(T));
        }
        _end = (_end + itemCount) % Size();
        _fill += itemCount;
        if (_fill > Size())
        {
            _fill = Size();
            _start = _end;
        }
    }
    itemsWritten = itemCount;
    return itemsWritten;
}

template<class T>
bool CyclicBuffer<T>::TryWrite(const T & item)
{
    if (_fill >= Size())
    {
        return false;
    }
    _buf[_end] = item;
    _end = (_end + 1) % Size();
    _fill++;
    if (_fill > Size())
    {
        _fill = Size();
        _start = _end;
    }
    return true;
}

template<class T>
void CyclicBuffer<T>::Flush()
{
    _start = 0;
    _end = 0;
    _fill = 0;
    _offset = 0;
}

} // namespace Parser
