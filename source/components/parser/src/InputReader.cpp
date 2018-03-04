#include "parser/InputReader.h"

#include <iostream>

using namespace std;
using namespace Parser;

static size_t BufferSize = 32768;

InputReader::InputReader(const std::string & path, std::istream & stream)
    : _stream(stream)
    , _sourceLocation(path)
    , _buffer(BufferSize)
{
    FillBuffer();
}

static bool IsStreamEOF(istream & stream)
{
    // Checking eof on invalid stream does not flag as eof, check whether good is set as well.
    return (stream.eof() || !stream.good());
}

bool InputReader::IsEOF() const
{
    return (IsStreamEOF(_stream) && _buffer.IsEmpty());
}

char InputReader::ReadChar()
{
    char ch {};
    if (_buffer.IsEmpty() && !IsEOF())
        FillBuffer();
    _buffer.Read(ch);
    _sourceLocation.UpdateOffset(_buffer.Offset());
    if (!IsEOF())
        _sourceLocation.NextChar();
    if (ch == '\n')
        _sourceLocation.NextLine();

    return ch;
}

string InputReader::ReadChars(size_t count)
{
    string result;
    size_t charsToRead = count;
    while (!IsEOF() && (charsToRead-- > 0))
    {
        char ch = ReadChar();
        if (ch != '\0')
            result += ch;
    }
    return result;
}

char InputReader::ReadAheadChar()
{
    assert(1 <= BufferSize);
    char ch;
    if ((_buffer.Fill() < 1) && !IsEOF())
        FillBuffer();
    size_t charsRead = _buffer.Peek(&ch, 1);
    if (charsRead == 0)
        ch = 0;

    return ch;
}

string InputReader::ReadAheadChars(size_t count)
{
    assert(count <= BufferSize);
    char buffer[count];
    if ((_buffer.Fill() < count) && !IsEOF())
        FillBuffer();
    size_t charsRead = _buffer.Peek(buffer, count);

    return string(buffer, charsRead);
}

void InputReader::FillBuffer()
{
    if (IsStreamEOF(_stream))
        return;
    char ch {};
    while (_buffer.Space() > 0)
    {
        _stream.read(&ch, 1);
        if (_stream.eof())
            break;
        _buffer.Write(ch);
    }
}

