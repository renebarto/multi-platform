#pragma once

#include "core/IDataStream.h"

namespace Core {

class ITextStream : public IDataStream
{
public:
    virtual char ReadChar() = 0;
    virtual std::string ReadLine() = 0;
    virtual bool ReadLine(std::string & line) = 0;
    virtual bool WriteLine(const std::string & line) = 0;
    virtual bool ReadString(std::string & value, const std::string & delimiters) = 0;
    virtual bool ReadAll(std::string & value) = 0;
};

} // namespace Core
