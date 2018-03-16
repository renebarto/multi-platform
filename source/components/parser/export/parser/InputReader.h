#pragma once

#include <iostream>
#include "parser/CyclicBuffer.h"
#include "parser/SourceLocation.h"

namespace Parser {

class InputReader
{
public:
    InputReader(const std::string & path, std::istream & stream);

    char ReadChar();
    std::string ReadChars(size_t count);
    char ReadAheadChar();
    std::string ReadAheadChars(size_t count);
    bool IsEOF() const;
    const SourceLocation Location() const { return _sourceLocation; }

protected:
    std::istream & _stream;
    SourceLocation _sourceLocation;
    CyclicBuffer<char> _buffer;

    void FillBuffer();
};

} // namespace Parser

