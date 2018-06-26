#pragma once

#include <iostream>
#include <string>
#include "core/files/BinaryStream.h"

namespace Core {
namespace Files {

class BinaryMemoryStreamBuf : public std::streambuf
{
public:
    BinaryMemoryStreamBuf(const std::streamsize bufferSize)
        : _dataBuffer()
        , _dataBufferOffset()
        , _buffer()
        , _bufferSize(bufferSize)
    {
        _buffer = new char[_bufferSize];
        setg(0, 0, 0);
        setp(_buffer, _buffer + _bufferSize);
    }

    virtual ~BinaryMemoryStreamBuf()
    {
        sync();
        delete [] _buffer;
    }

    virtual std::streambuf::int_type underflow()
    {
        // Read from buffer
        _dataBufferOffset = std::min(static_cast<std::streamsize>(_dataBuffer.Size()), _dataBufferOffset);
        std::streamsize charsRead = std::min(_bufferSize, static_cast<std::streamsize>(_dataBuffer.Size() - _dataBufferOffset));
        if (!charsRead)
            return traits_type::eof();
        std::copy(_dataBuffer.Data() + _dataBufferOffset,
                  _dataBuffer.Data() + _dataBufferOffset + charsRead,
                  _buffer);
        _dataBufferOffset += charsRead;

        setg(_buffer, _buffer, _buffer + charsRead);
        return traits_type::to_int_type(*gptr());
    }

    virtual std::streambuf::int_type overflow(std::streambuf::int_type value)
    {
        std::streamsize write = pptr() - pbase();
        if (write)
        {
            // Write to original buffer
            _dataBuffer.Append(OSAL::ByteArray(_buffer, static_cast<size_t>(write)));
            pbump(write);
        }

        setp(_buffer, _buffer + _bufferSize);
        if (!traits_type::eq_int_type(value, traits_type::eof()))
            sputc(value);
        return traits_type::not_eof(value);
    };

    virtual int sync()
    {
        std::streambuf::int_type result = this->overflow(traits_type::eof());
        if (pbase() == pptr())
            return 0;
        pubsync();
        return traits_type::eq_int_type(result, traits_type::eof()) ? -1 : 0;
    }
    const OSAL::ByteArray & GetContents() const { return _dataBuffer; }
    void SetContents(const OSAL::ByteArray & value)
    {
        _dataBuffer = value;
        _dataBufferOffset = 0;
        if (_dataBuffer.Size() > 0)
        {
            std::streamsize numBytes = std::min(_bufferSize, static_cast<std::streamsize>(_dataBuffer.Size()));
            std::copy(_dataBuffer.Data(), _dataBuffer.Data() + numBytes, _buffer);
            setg(_buffer, _buffer, _buffer + numBytes);
            _dataBufferOffset = numBytes;
        }
    }

private:
    OSAL::ByteArray _dataBuffer;
    std::streamsize _dataBufferOffset;
    char * _buffer;
    std::streamsize _bufferSize;
};

class BinaryMemoryStream : virtual public BinaryStream
{
public:
    BinaryMemoryStream();
    explicit BinaryMemoryStream(const OSAL::ByteArray & value);
    virtual ~BinaryMemoryStream();

    BinaryMemoryStream(const BinaryMemoryStream &) = delete;
    BinaryMemoryStream(BinaryMemoryStream &&) = delete;
    BinaryMemoryStream & operator = (const BinaryMemoryStream &) = delete;
    BinaryMemoryStream & operator = (BinaryMemoryStream &&) = delete;

    const OSAL::ByteArray & GetContents() const { return reinterpret_cast<BinaryMemoryStreamBuf *>(_stream.rdbuf())->GetContents(); }
    void SetContents(const OSAL::ByteArray & value) { reinterpret_cast<BinaryMemoryStreamBuf *>(_stream.rdbuf())->SetContents(value); }
    void Flush();
    bool CompareTo(const BinaryMemoryStream & other) const;

protected:
    std::iostream _stream;
};

} // namespace Files
} // namespace Core
