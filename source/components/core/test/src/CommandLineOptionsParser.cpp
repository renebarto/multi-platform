#include "CommandLineOptionsParser.h"

CommandLineOptionsParser::CommandLineOptionsParser(OSAL::Console & console)
    : Core::CommandLineParser(console, "Test runner", "Runs tests")
    , testSuiteName()
    , testFixtureName()
    , testName()
    , xmlOutput()
{
    AddOptionRequiredArgument('s', "runsuite", "Run only specified test suite (default = all)",
                              testSuiteName);
    AddOptionRequiredArgument('f', "runfixture",  "Run only specified test fixture (default = all)",
                              testFixtureName);
    AddOptionRequiredArgument('t', "runtest", "Run only specified test (default = all)",
                              testName);
    AddOptionRequiredArgument("xmloutput", "Send output to specified XML file",
                              xmlOutput);
}

