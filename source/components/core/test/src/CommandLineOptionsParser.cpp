#include "CommandLineOptionsParser.h"

CommandLineOptionsParser::CommandLineOptionsParser(OSAL::Console & console)
        : Core::CommandLineParser(console, "Test runner", "Runs tests")
        , testFilter("")
        , gtestFilter("")
        , testList(false)
        , gtestList(false)
        , testColor(true)
        , gtestColor(1)
        , gtestEmulation(false)
        , xml()
{
    Core::CommandLineOptionGroup::Ptr group =
            std::make_shared<Core::CommandLineOptionGroup>("Test runner", "Runs tests");
    group->AddOptionRequiredArgument("filter", 0,
                                     "Use filter for tests (default = *)",
                                     testFilter);
    group->AddOptionRequiredArgument("gtest_filter", 0,
                                     "Use filter for tests (default = *) (Google Test)",
                                     gtestFilter);
    group->AddSwitchWithVariable("list", testList, false, "List selected tests");
    group->AddSwitchWithVariable("gtest_list_tests", gtestList, false, "List selected tests (Google Test)");
    group->AddSwitchWithVariable("color", testColor, true, "Color output");
    group->AddSwitchWithVariable("nocolor", testColor, false, "No color output");
    group->AddOptionOptionalArgumentWithVariable("gtest_color", 0, "Color output (Google Test)", gtestColor);
    group->AddSwitchWithVariable("gtest_emulation", gtestEmulation, false, "Google Test emulation");
    group->AddOptionRequiredArgument("xml", 0, "Send output to specified XML file",
                                     xml);
    AddGroup(group);
}
