#include "core/CommandLineOptionGroup.h"

using namespace std;
using namespace Core;

CommandLineOptionGroup::CommandLineOptionGroup(const string & name, const string description /*= ""*/)
    : name(name)
    , description(description)
    , options()
{
}

CommandLineOptionGroup::~CommandLineOptionGroup()
{
    options.clear();
}

void CommandLineOptionGroup::AddOption(const CommandLineOption::Ptr option)
{
    options.push_back(option);
}
