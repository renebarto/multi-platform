#include "CommandLineOptionsParser.h"

CommandLineOptionsParser::CommandLineOptionsParser(OSAL::Console & console)
    : Core::CommandLineParser(console, "Test runner", "Runs tests")
    , testSuiteName()
    , testFixtureName()
    , testName()
    , xmlOutput()
{
    Core::CommandLineOptionGroup::Ptr group =
        std::make_shared<Core::CommandLineOptionGroup>("Test runner", "Runs tests");
    group->AddOptionRequiredArgument("runsuite", 's',
                                     "Run only specified test suite (default = all)",
                                     testSuiteName);
    group->AddOptionRequiredArgument("runfixture", 'f',
                                     "Run only specified test fixture (default = all)",
                                     testFixtureName);
    group->AddOptionRequiredArgument("runtest", 't',
                                     "Run only specified test (default = all)", testName);
    group->AddOptionRequiredArgument("xmloutput", 0, "Send output to specified XML file",
                                     xmlOutput);
    AddGroup(group);
}
