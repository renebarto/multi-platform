#include "core/CommandLineOptionGroup.h"

using namespace std;
using namespace Core;

CommandLineOptionGroup::CommandLineOptionGroup(const string & name, const string description /*= ""*/)
    : _name(name)
    , _description(description)
    , _options()
{
}

CommandLineOptionGroup::~CommandLineOptionGroup()
{
    _options.clear();
}

void CommandLineOptionGroup::AddOption(const CommandLineOption::Ptr option)
{
    _options.push_back(option);
}
