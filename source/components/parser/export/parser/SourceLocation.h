#pragma once

#include <iostream>

namespace Parser {

struct FSA
{
    FSA()
            : fileName()
            , line(1)
            , column(1)
            , fileOffset()
    {}
    explicit FSA(const std::string & aFileName)
            : fileName(aFileName)
            , line(1)
            , column(1)
            , fileOffset()
    {}
    FSA(const FSA & other)
            : fileName(other.fileName)
            , line(other.line)
            , column(other.column)
            , fileOffset(other.fileOffset)
    {}
    FSA(FSA && other)
            : fileName(std::move(other.fileName))
            , line(other.line)
            , column(other.column)
            , fileOffset(other.fileOffset)
    {
        other.fileName = {};
        other.line = {1};
        other.column = {1};
        other.fileOffset = {};
    }
    FSA(const std::string & aFileName, unsigned aLine, unsigned aColumn, ssize_t aFileOffset)
            : fileName(aFileName)
            , line(aLine)
            , column(aColumn)
            , fileOffset(aFileOffset)
    {
    }

    FSA & operator = (const FSA & other)
    {
        if (this != &other)
        {
            fileName = other.fileName;
            line = other.line;
            column = other.column;
            fileOffset = other.fileOffset;
        }
        return *this;
    }
    FSA & operator = (FSA && other)
    {
        if (this != &other)
        {
            fileName = std::move(other.fileName);
            line = other.line;
            column = other.column;
            fileOffset = other.fileOffset;
            other.fileName = {};
            other.line = {1};
            other.column = {1};
            other.fileOffset = {};
        }
        return *this;
    }

    void NextChar() { ++column; }
    void NextLine() { ++line; column = 1; }
    void UpdateOffset(ssize_t offset) { fileOffset = offset; }

    std::string fileName;
    unsigned line;
    unsigned column;
    ssize_t fileOffset;
};

} // namespace Parser

inline bool operator == (const Parser::FSA & lhs, const Parser::FSA & rhs)
{
    return (lhs.fileName == rhs.fileName) &&
           (lhs.line == rhs.line) &&
           (lhs.column == rhs.column) &&
           (lhs.fileOffset == rhs.fileOffset);
}

inline bool operator != (const Parser::FSA & lhs, const Parser::FSA & rhs)
{
    return ! operator == (lhs, rhs);
}

inline std::ostream & operator << (std::ostream & stream, const Parser::FSA & location)
{
    stream << location.fileName << ":" << location.line << ":" << location.column;
    if (location.fileOffset < 0)
        stream << " (EOF)";
    else
        stream << " (" << location.fileOffset << ")";
    return stream;
}

