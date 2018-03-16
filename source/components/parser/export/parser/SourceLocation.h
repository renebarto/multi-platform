#pragma once

#include <iostream>

namespace Parser {

struct SourceLocation
{
    SourceLocation()
            : fileName()
            , line(1)
            , column(1)
            , fileOffset()
    {}
    explicit SourceLocation(const std::string & aFileName)
            : fileName(aFileName)
            , line(1)
            , column(1)
            , fileOffset()
    {}
    SourceLocation(const SourceLocation & other)
            : fileName(other.fileName)
            , line(other.line)
            , column(other.column)
            , fileOffset(other.fileOffset)
    {}
    SourceLocation(SourceLocation && other)
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
    SourceLocation(const std::string & aFileName, unsigned aLine, unsigned aColumn, ssize_t aFileOffset)
            : fileName(aFileName)
            , line(aLine)
            , column(aColumn)
            , fileOffset(aFileOffset)
    {
    }

    SourceLocation & operator = (const SourceLocation & other)
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
    SourceLocation & operator = (SourceLocation && other)
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

inline bool operator == (const Parser::SourceLocation & lhs, const Parser::SourceLocation & rhs)
{
    return (lhs.fileName == rhs.fileName) &&
           (lhs.line == rhs.line) &&
           (lhs.column == rhs.column) &&
           (lhs.fileOffset == rhs.fileOffset);
}

inline bool operator != (const Parser::SourceLocation & lhs, const Parser::SourceLocation & rhs)
{
    return ! operator == (lhs, rhs);
}

inline std::ostream & operator << (std::ostream & stream, const Parser::SourceLocation & location)
{
    stream << location.fileName << ":" << location.line << ":" << location.column;
    if (location.fileOffset < 0)
        stream << " (EOF)";
    else
        stream << " (" << location.fileOffset << ")";
    return stream;
}

