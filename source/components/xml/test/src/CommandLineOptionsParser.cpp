#include "CommandLineOptionsParser.h"

CommandLineOptionsParser::CommandLineOptionsParser(OSAL::Console & console)
    : Core::CommandLineParser(console, _("Test runner"), _("Runs tests"))
    , testSuiteName()
    , testFixtureName()
    , testName()
    , xmlOutput()
{
    Core::CommandLineOptionGroup::Ptr group =
        std::make_shared<Core::CommandLineOptionGroup>(_("Test runner"), _("Runs tests"));
    group->AddOptionRequiredArgument(_("runsuite"), _('s'),
                                     _("Run only specified test suite (default = all)"),
                                     testSuiteName);
    group->AddOptionRequiredArgument(_("runfixture"), _('f'),
                                     _("Run only specified test fixture (default = all)"),
                                     testFixtureName);
    group->AddOptionRequiredArgument(_("runtest"), _('t'),
                                     _("Run only specified test (default = all)"), testName);
    group->AddOptionRequiredArgument(_("xmloutput"), 0, _("Send output to specified XML file"),
                                     xmlOutput);
    AddGroup(group);
}

