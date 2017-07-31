#pragma once

#include <core/CommandLineParser.h>

class CommandLineOptionsParser : public Core::CommandLineParser
{
public:
    CommandLineOptionsParser(OSAL::Console & console);

    OSAL::String testSuiteName;
    OSAL::String testFixtureName;
    OSAL::String testName;
    OSAL::String xmlOutput;
};


