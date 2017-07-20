#pragma once

#include <core/CommandLineParser.h>

class CommandLineOptionsParser : public Core::CommandLineParser
{
public:
    CommandLineOptionsParser(OSAL::Console & console);

    std::string testSuiteName;
    std::string testFixtureName;
    std::string testName;
    std::string xmlOutput;
};


