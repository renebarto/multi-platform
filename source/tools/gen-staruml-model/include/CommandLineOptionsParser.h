#pragma once

#include <core/CommandLineParser.h>

class CommandLineOptionsParser : public Core::CommandLineParser
{
public:
    CommandLineOptionsParser(OSAL::Console & console);

    OSAL::String sourceFile;
    OSAL::String modelFile;
};


