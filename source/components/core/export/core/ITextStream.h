#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <osal/FlagOperators.h>
#include "core/files/File.h"

namespace Core {

class ITextStream
{
public:
    virtual ~ITextStream() {}

    virtual char ReadChar() = 0;
    virtual bool ReadChar(char & ch) = 0;
    virtual std::string ReadLine() = 0;
    virtual bool ReadLine(std::string & line) = 0;
    virtual bool WriteChar(char ch) = 0;
    virtual bool WriteLine(const std::string & line) = 0;

    // TODO: Add more methods to interface
};

} // namespace Core
