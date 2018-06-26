#pragma once

#include <iostream>
#include <string>
#include "core/files/TextStream.h"

namespace Core {
namespace Files {

class TextMemoryStreamBuf : public std::streambuf
{
public:
    TextMemoryStreamBuf(const std::streamsize bufferSize)
        : _stringBuffer()
        , _stringBufferOffset()
        , _buffer()
        , _bufferSize(bufferSize)
    {
        _buffer = new char[_bufferSize];
        setg(0, 0, 0);
        setp(_buffer, _buffer + _bufferSize);
    }

    virtual ~TextMemoryStreamBuf()
    {
        sync();
        delete [] _buffer;
    }

    virtual std::streambuf::int_type underflow()
    {
        // Read from buffer
        _stringBufferOffset = std::min(static_cast<std::streamsize>(_stringBuffer.length()), _stringBufferOffset);
        std::streamsize charsRead = std::min(_bufferSize, static_cast<std::streamsize>(_stringBuffer.length() - _stringBufferOffset));
        if (!charsRead)
            return traits_type::eof();
        std::copy(_stringBuffer.data() + _stringBufferOffset,
                  _stringBuffer.data() + _stringBufferOffset + charsRead,
                  _buffer);
        _stringBufferOffset += charsRead;

        setg(_buffer, _buffer, _buffer + charsRead);
        return traits_type::to_int_type(*gptr());
    }

    virtual std::streambuf::int_type overflow(std::streambuf::int_type value)
    {
        std::streamsize write = pptr() - pbase();
        if (write)
        {
            // Write to original buffer
            _stringBuffer.append(_buffer, static_cast<size_t>(write));
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
    const std::string & GetContents() const { return _stringBuffer; }
    void SetContents(const std::string & value)
    {
        _stringBuffer = value;
        _stringBufferOffset = 0;
    }

private:
    std::string _stringBuffer;
    std::streamsize _stringBufferOffset;
    char * _buffer;
    std::streamsize _bufferSize;
};

class TextMemoryStream : virtual public TextStream
{
public:
    TextMemoryStream();
    explicit TextMemoryStream(const std::string & value);
    virtual ~TextMemoryStream();

    TextMemoryStream(const TextMemoryStream &) = delete;
    TextMemoryStream(TextMemoryStream &&) = delete;
    TextMemoryStream & operator = (const TextMemoryStream &) = delete;
    TextMemoryStream & operator = (TextMemoryStream &&) = delete;

    const std::string & GetContents() const { return reinterpret_cast<TextMemoryStreamBuf *>(_stream.rdbuf())->GetContents(); }
    void SetContents(const std::string & value) { reinterpret_cast<TextMemoryStreamBuf *>(_stream.rdbuf())->SetContents(value); }
    void Flush();
    bool CompareTo(const TextMemoryStream & other) const;

protected:
    std::iostream _stream;
};

} // namespace Files
} // namespace Core
