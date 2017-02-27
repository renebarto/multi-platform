#include "CommandLineOptionsParser.h"

CommandLineOptionsParser::CommandLineOptionsParser(OSAL::Console & console)
    : Core::CommandLineParser(console, "Test runner", "Runs tests")
    , testSuiteName()
    , testFixtureName()
    , testName()
    , xmlOutput()
{
    AddOptionRequiredArgument("runsuite", 's', "Run only specified test suite (default = all)",
                              testSuiteName);
    AddOptionRequiredArgument("runfixture", 'f', "Run only specified test fixture (default = all)",
                              testFixtureName);
    AddOptionRequiredArgument("runtest", 't', "Run only specified test (default = all)",
                              testName);
    AddOptionRequiredArgument("xmloutput", "Send output to specified XML file",
                              xmlOutput);
}

