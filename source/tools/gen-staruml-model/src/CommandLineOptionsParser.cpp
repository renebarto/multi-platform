#include "CommandLineOptionsParser.h"

CommandLineOptionsParser::CommandLineOptionsParser(OSAL::Console & console)
    : Core::CommandLineParser(console, _("Test runner"), _("Runs tests"))
    , sourceFile()
    , modelFile()
{
    Core::CommandLineOptionGroup::Ptr group =
        std::make_shared<Core::CommandLineOptionGroup>(_("Test runner"), _("Runs tests"));
    group->AddOptionRequiredArgument(_("source"), _('i'),
                                     _("Source file to parse"), sourceFile);
    group->AddOptionRequiredArgument(_("model"), _('m'),
                                     _("File to save generated model to (default: output on stdout)"), modelFile);
    AddGroup(group);
}

