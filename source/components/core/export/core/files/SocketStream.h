#pragma once

#include <iostream>

namespace Core {
namespace Files {

class StreamBuf : public std::streambuf
{
public:
    StreamBuf(std::streambuf * buffer, const size_t bufferSize)
        : _buffer(buffer)
        , _bufferData()
        , _bufferSize(bufferSize)
    {
        _bufferData = new char[_bufferSize];
        setg(0, 0, 0);
        setp(_bufferData, _bufferData + _bufferSize);
    }

    virtual ~StreamBuf()
    {
        sync();
        delete[] _bufferData;
    }

    virtual std::streambuf::int_type underflow()
    {
        // Read line from original buffer
        std::streamsize read = _buffer->sgetn(_bufferData, _bufferSize);
        if (!read)
            return traits_type::eof();

        // Do vertical XOR decoding

        setg(_bufferData, _bufferData, _bufferData + _bufferSize);
        return traits_type::to_int_type(*gptr());
    }

    virtual std::streambuf::int_type overflow(std::streambuf::int_type value)
    {
        size_t write = pptr() - pbase();
        if (write)
        {
            // Write line to original buffer
            std::streamsize written = _buffer->sputn(_bufferData, write);
            if (written != write)
                return traits_type::eof();
        }

        setp(_bufferData, _bufferData + _bufferSize);
        if (!traits_type::eq_int_type(value, traits_type::eof()))
            sputc(value);
        return traits_type::not_eof(value);
    };

    virtual int sync()
    {
        std::streambuf::int_type result = this->overflow(traits_type::eof());
        _buffer->pubsync();
        return traits_type::eq_int_type(result, traits_type::eof()) ? -1 : 0;
    }

private:
    std::streambuf * _buffer;
    char * _bufferData;
    size_t _bufferSize;
};

class IStream: public std::istream
{
public:
    IStream(std::istream & stream, const size_t bufferSize) :
        std::istream(new StreamBuf(stream.rdbuf(), bufferSize))
    {}

    virtual ~IStream()
    {
        delete rdbuf();
    }
};


class OStream: public std::ostream
{
public:
    OStream(std::ostream &stream, const size_t bufferSize) :
        std::ostream(new StreamBuf(stream.rdbuf(), bufferSize))
    {}

    virtual ~OStream()
    {
        delete rdbuf();
    }
};

class IOStream: public std::iostream
{
public:
    IOStream(std::iostream &stream, const size_t bufferSize)
        : std::iostream(new StreamBuf(stream.rdbuf(), bufferSize))
    {}

    virtual ~IOStream()
    {
        delete rdbuf();
    }
};

} // namespace Files
} // namespace Core
