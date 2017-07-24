#pragma once

#include "core/Core.h"
#include <stdexcept>

namespace XMLParser
{

enum class XMLIgnoreCase
{
    IgnoreCase,
    MatchCase,
};

enum class XMLEncoding
{
    Unknown,
    UTF8,
    Legacy,
};

extern const XMLEncoding XMLDefaultEncoding;

class XMLCursor
{
public:
    XMLCursor()
    {
        Clear();
    }

    XMLCursor(int row, int col)
    {
        Set(row, col);
    }

    void Clear()
    {
        row = col = -1;
    }

    void Set(int row, int col)
    {
        this->row = row;
        this->col = col;
    }

    int Row() const
    {
        return row;
    }

    int Col() const
    {
        return col;
    }

protected:
    int row;
    int col;
};

} // namespace XMLParser
