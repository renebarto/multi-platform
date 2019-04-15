#pragma once

#include <core/CommandLineParser.h>

class CommandLineOptionsParser : public Core::CommandLineParser
{
public:
    CommandLineOptionsParser(OSAL::Console & console);

    std::string testFilter;
    std::string gtestFilter;
    bool testList;
    bool gtestList;
    bool testColor;
    int gtestColor;
    bool gtestEmulation;
    std::string xml;

    bool GoogleTestEmulation() { gtestEmulation |= !gtestFilter.empty(); return gtestEmulation; }
    std::string Filter() { gtestEmulation |= !gtestFilter.empty(); return gtestEmulation ? gtestFilter : testFilter; }
    bool List() { gtestEmulation |= !gtestFilter.empty(); return gtestEmulation ? gtestList : testList; }
    bool Color() { gtestEmulation |= !gtestFilter.empty(); return gtestEmulation ? gtestColor != 0 : testColor; }
    std::string XmlOutput() { return xml; }
};


