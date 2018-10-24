#include "CommandLineOptionsParser.h"

CommandLineOptionsParser::CommandLineOptionsParser(OSAL::Console & console)
        : Core::CommandLineParser(console, "Test runner", "Runs tests")
        , testSuiteName()
        , testFixtureName()
        , testName()
        , testFilter()
        , testColor()
        , xmlOutput()
{
    Core::CommandLineOptionGroup::Ptr group
            = std::make_shared<Core::CommandLineOptionGroup>("Test runner", "Runs tests");
    group->AddOptionRequiredArgument(
            "runsuite", 's', "Run only specified test suite (default = all)", testSuiteName);
    group->AddOptionRequiredArgument(
            "runfixture", 'f', "Run only specified test fixture (default = all)", testFixtureName);
    group->AddOptionRequiredArgument(
            "runtest", 't', "Run only specified test (default = all)", testName);
    group->AddOptionRequiredArgument(
            "gtest_filter",
            0,
            "Google test filter expression (currently only used to switch to google test emulation)",
            testFilter);
    group->AddOptionRequiredArgument(
            "gtest_color",
            0,
            "Google test color output (currently only used to switch to google test emulation)",
            testColor);
    group->AddOptionRequiredArgument("xmloutput", 0, "Send output to specified XML file", xmlOutput);
    AddGroup(group);
}
