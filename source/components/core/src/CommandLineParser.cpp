#include "core/CommandLineParser.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include "osal/OSAL.h"
#include "osal/Path.h"
#include "osal/Strings.h"

using namespace std;
using namespace Core;

const OSAL::String CommandLineParser::DefaultMainOptionsGroupName = _("Main");
const OSAL::String CommandLineParser::HelpOptionsGroupName = _("Help Options");

CommandLineParser::CommandLineParser(OSAL::Console & console,
                                     const OSAL::String & name,
                                     const OSAL::String & description)
    : _groups()
    , _mainGroup()
    , _console(console)
    , _autoHelp(true)
    , _showHelp(false)
    , _name(name)
    , _description(description)
    , _nonOptions()
    , _options()
    , _getOptData()
{
    SetupStandardGroups();
}

CommandLineParser::~CommandLineParser()
{
    _nonOptions.clear();
    _groups.clear();
}

bool CommandLineParser::HaveOption(OSAL::Char shortName) const
{
    size_t index = MatchShortOption(shortName);
    return (index < numeric_limits<size_t>::max());
}

bool CommandLineParser::HaveOption(OSAL::String longName) const
{
    size_t index = MatchLongOption(longName.c_str(), longName.length());
    return (index < numeric_limits<size_t>::max());
}

bool CommandLineParser::FoundOption(OSAL::Char shortName) const
{
    size_t index = MatchShortOption(shortName);
    return (index < numeric_limits<size_t>::max()) && _options[index]->FoundOption();
}

bool CommandLineParser::FoundOption(OSAL::String longName) const
{
    size_t index = MatchLongOption(longName.c_str(), longName.length());
    return (index < numeric_limits<size_t>::max()) && _options[index]->FoundOption();
}

OSAL::String CommandLineParser::GetOption(OSAL::Char shortName) const
{
    size_t index = MatchShortOption(shortName);
    return (index < numeric_limits<size_t>::max())
           ? _options[index]->Argument()
		: OSAL::String();
}

OSAL::String CommandLineParser::GetOption(OSAL::String longName) const
{
    size_t index = MatchLongOption(longName.c_str(), longName.length());
    return (index < numeric_limits<size_t>::max())
           ? _options[index]->Argument()
           : OSAL::String();
}

bool CommandLineParser::Parse(int argc, const OSAL::Char * argv[])
{
    try
    {
        bool result = InternalParse(argc, argv);
        if (!result)
        {
            if (_autoHelp)
            {
                ShowHelp(argv[0]);
            }
            return false;
        }
    }
    catch (const exception & e)
    {
        _console << fgcolor(OSAL::ConsoleColor::Red) << _("Exception: ") << e.what() << endl;
        if (_autoHelp)
        {
            ShowHelp(argv[0]);
        }
        return false;
    }
    return true;
}

bool CommandLineParser::InternalParse(int argc, const OSAL::Char * argv[])
{
    size_t argCount = size_t(argc);
    FillOptionsList();
    OSAL::String optionString;
    for (auto opt : _options)
    {
        auto optionWithVariable = dynamic_pointer_cast<CommandLineSwitchWithVariable<int>>(opt);
        if (!optionWithVariable)
        {
            if (opt->ShortName())
            {
                optionString += opt->ShortName();
                if (opt->ArgType() != CommandLineArgumentType::NoArgument)
                    optionString += ':';
                if (opt->ArgType() == CommandLineArgumentType::OptionalArgument)
                    optionString += ':';
            }
        }
    }

    int result = 0;
    size_t optionIndex;
    _getOptData.Initialize();
    while (result != -1)
    {
        result = GetOpt(static_cast<size_t>(argc), argv, optionString, optionIndex);
        if (result == 0)
        {
            CommandLineOption::Ptr option = _options[optionIndex];
            auto optionWithVariable = dynamic_pointer_cast<CommandLineSwitchWithVariable<int>>(option);
            if (optionWithVariable)
            {
                SelectOption(option, nullptr);
            } else
            {
                SelectOption(option, _getOptData.optionArgument);
            }
        } else if (result == '?')
        {
            break;
        } else if (result != -1)
        {
            size_t index = MatchShortOption(static_cast<OSAL::Char>(result));
            if (index < numeric_limits<size_t>::max())
            {
                CommandLineOption::Ptr option = _options[index];
                switch (option->ArgType())
                {
                    case CommandLineArgumentType::NoArgument:
                        SelectOption(option, nullptr);
                        break;
                    case CommandLineArgumentType::OptionalArgument:
                        SelectOption(option, _getOptData.optionArgument);
                        break;
                    case CommandLineArgumentType::RequiredArgument:
                        SelectOption(option, _getOptData.optionArgument);
                        break;
                    default:
                        break;
                }
            }
        }

    }

    while (_getOptData.optionIndex < argCount)
    {
        AddNonOption(argv[_getOptData.optionIndex]);
        _getOptData.optionIndex++;
    }

    return (result != '?');
}

void CommandLineParser::ExchangeOption(const OSAL::Char ** argv)
{
    size_t bottom = _getOptData.firstNonOption;
    size_t middle = _getOptData.lastNonOption;
    size_t top = _getOptData.optionIndex;
    const OSAL::Char * tmp;
    while ((top > middle) && (middle > bottom))
    {
        if ((top - middle) > (middle - bottom))
        {
            size_t len = middle - bottom;
            for (size_t i = 0; i < len; i++)
            {
                tmp = argv[bottom + i];
                argv[bottom + i] = argv[top - (middle - bottom) + i];
                argv[top - (middle - bottom) + i] = tmp;
            }
            top -= len;
        }
        else
        {
            size_t len = top - middle;
            for (size_t i = 0; i < len; i++)
            {
                tmp = argv[bottom + i];
                argv[bottom + i] = argv[middle + i];
                argv[middle + i] = tmp;
            }
            bottom += len;
        }
    }
    _getOptData.firstNonOption += (_getOptData.optionIndex - _getOptData.lastNonOption);
    _getOptData.lastNonOption = _getOptData.optionIndex;
}

bool CommandLineParser::AtNonOption(size_t argCount, const OSAL::Char ** argv) const
{
    return (_getOptData.optionIndex < argCount) &&
           (('-' != argv[_getOptData.optionIndex][0]) || ('\0' == argv[_getOptData.optionIndex][1]));
}

bool CommandLineParser::AtLongOption(size_t argCount, const OSAL::Char ** argv) const
{
    return (_getOptData.optionIndex < argCount) &&
           (('-' == argv[_getOptData.optionIndex][0]) && ('-' == argv[_getOptData.optionIndex][1]));
}

int CommandLineParser::HandleLongOption(size_t argCount, const OSAL::Char ** argv, const OSAL::Char * optionString,
                                        size_t & optionIndex, bool printErrors)
{
    // Long option
    const OSAL::Char * nameEnd;
    size_t nameLength;
    CommandLineOption::Ptr optionFound = nullptr;
    struct OptionList
    {
        CommandLineOption::Ptr option;
        struct OptionList * next;
    } * amiguousOptionsList = nullptr;

    bool exact = false;
    size_t indexFound {};
    for (nameEnd = _getOptData.nextChar; *nameEnd && ('=' != *nameEnd); nameEnd++)
        ;

    nameLength = static_cast<size_t>(nameEnd - _getOptData.nextChar);
    size_t currentOptionIndex = 0;
    for (auto currentOption : _options)
    {
        if (!OSAL::Strings::strcmp(currentOption->LongName().c_str(), _getOptData.nextChar, nameLength))
        {
            auto currentOptionWithVariable = dynamic_pointer_cast<CommandLineSwitchWithVariable<int>>(currentOption);
            auto optionFoundWithVariable = dynamic_pointer_cast<CommandLineSwitchWithVariable<int>>(optionFound);
            if (nameLength == currentOption->LongName().length())
            {
                optionFound = currentOption;
                indexFound = currentOptionIndex;
                exact = true;
                break;
            } else if (nullptr == optionFound)
            {
                optionFound = currentOption;
                indexFound = currentOptionIndex;
            } else if (currentOption->IsNotEqual(optionFound))
            {
                struct OptionList *newOption = new OptionList;
                newOption->option = currentOption;
                newOption->next = amiguousOptionsList;
                amiguousOptionsList = newOption;
            }
        }
        currentOptionIndex++;
    }
    if ((nullptr != amiguousOptionsList) && !exact)
    {
        if (printErrors)
        {
            struct OptionList first;
            first.option = optionFound;
            first.next = amiguousOptionsList;
            amiguousOptionsList = &first;
            _console << fgcolor(OSAL::ConsoleColor::Red) << argv[0] << ": option '" << argv[_getOptData.optionIndex] << "' is ambiguous; possibilities:";
            do
            {
                _console << " '--" << amiguousOptionsList->option->LongName() << "'";
                amiguousOptionsList = amiguousOptionsList->next;
            }
            while (nullptr != amiguousOptionsList);
            _console << fgcolor(OSAL::ConsoleColor::Default) << endl;
        }
        _getOptData.nextChar += OSAL::Strings::strlen(_getOptData.nextChar);
        _getOptData.optionIndex++;
        _getOptData.optopt = 0;
        return '?';
    }
    if (nullptr != optionFound)
    {
        currentOptionIndex = indexFound;
        _getOptData.optionIndex++;
        if (*nameEnd)
        {
            // We have an argument after the equals sign.
            if (CommandLineArgumentType::NoArgument != optionFound->ArgType())
                _getOptData.optionArgument = nameEnd + 1;
            else
            {
                if (printErrors)
                {
                    _console << fgcolor(OSAL::ConsoleColor::Red);
                    if ('-' == argv[_getOptData.optionIndex - 1][1])
                    {
                        _console << argv[0] << _(": option '--") << optionFound->LongName() << _("' doesn't allow an argument");
                    }
                    else
                    {
                        _console << argv[0] << _(": option '--") << argv[_getOptData.optionIndex - 1][0] << optionFound->LongName() << _("' doesn't allow an argument");
                    }
                    _console << fgcolor(OSAL::ConsoleColor::Default) << endl;
                }
                _getOptData.nextChar += OSAL::Strings::strlen(_getOptData.nextChar);
                auto optionFoundWithVariable = dynamic_pointer_cast<CommandLineSwitchWithVariable<int>>(optionFound);
                _getOptData.optopt = optionFound->ShortName();
                if (optionFoundWithVariable)
                {
                    _getOptData.optopt = optionFoundWithVariable->Value();
                }
                return '?';
            }
        }
        else if (CommandLineArgumentType::RequiredArgument == optionFound->ArgType())
        {
            if (_getOptData.optionIndex < argCount)
                _getOptData.optionArgument = argv[_getOptData.optionIndex++];
            else
            {
                if (printErrors)
                {
                    _console << fgcolor(OSAL::ConsoleColor::Red);
                    _console <<  argv[0] << ": option '--" << optionFound->LongName() << "' requires an argument";
                    _console << fgcolor(OSAL::ConsoleColor::Default) << endl;
                }
                _getOptData.nextChar += OSAL::Strings::strlen(_getOptData.nextChar);
                _getOptData.optopt = optionFound->ShortName();
                return optionString[0] == ':' ? ':' : '?';
            }
        }
        _getOptData.nextChar += OSAL::Strings::strlen(_getOptData.nextChar);
        optionIndex = currentOptionIndex;
        optionFound->SetOptionFound();
        if (optionFound->IsSwitchWithVariable())
        {
            return 0;
        }
        return optionFound->ShortName();
    }
    if (printErrors)
    {
        _console << fgcolor(OSAL::ConsoleColor::Red);
        if ('-' == argv[_getOptData.optionIndex][1])
        {
            _console <<  argv[0] << _(": unrecognized option '--") << _getOptData.nextChar << _("'");
        }
        else
        {
            _console <<  argv[0] << _(": unrecognized option '--") << argv[_getOptData.optionIndex][0] << _getOptData.nextChar << _("'");
        }
        _console << fgcolor(OSAL::ConsoleColor::Default) << endl;
    }
    _getOptData.nextChar = _("");
    _getOptData.optionIndex++;
    _getOptData.optopt = 0;
    return _('?');
}

void CommandLineParser::SetArgument()
{
    if ('=' == *_getOptData.nextChar)
        _getOptData.nextChar++;
    _getOptData.optionArgument = _getOptData.nextChar;
    _getOptData.optionIndex++;
}

int CommandLineParser::HandleShortOption(size_t argCount, const OSAL::Char ** argv, const OSAL::Char * optionString,
                                         bool printErrors)
{
    OSAL::Char c = *_getOptData.nextChar++;
    const OSAL::Char * foundShortOption = OSAL::Strings::strchr(optionString, c);
    if (_('\0') == *_getOptData.nextChar)
        // No argument in this command line parameter
        ++_getOptData.optionIndex;
    if ((nullptr == foundShortOption) || (':' == c))
    {
        if (printErrors)
        {
            _console << fgcolor(OSAL::ConsoleColor::Red);
            _console <<  argv[0] << ": invalid option -- '" << c << "'";
            _console << fgcolor(OSAL::ConsoleColor::Default) << endl;
        }
        _getOptData.optopt = c;
        return '?';
    }
    if (foundShortOption[1] == ':')
    {
        // We could expect an argument
        if (foundShortOption[2] == ':')
        {
            // The argument is optional
            if ('\0' != *_getOptData.nextChar)
                // We have an argument
                SetArgument();
            else
                // No argument
                _getOptData.optionArgument = nullptr;
            _getOptData.nextChar = nullptr;
        }
        else
        {
            // The argument is required
            if ('\0' != *_getOptData.nextChar)
                // We have an argument in this command line parameter
                SetArgument();
            else if (_getOptData.optionIndex == argCount)
            {
                // No argument
                if (printErrors)
                {
                    _console << fgcolor(OSAL::ConsoleColor::Red);
                    _console <<  argv[0] << _(": option '--") << c << _("' requires an argument");
                    _console << fgcolor(OSAL::ConsoleColor::Default) << endl;
                }
                _getOptData.optopt = c;
                c = (optionString[0] == _(':')) ? _(':') : _('?');
            }
            else
                // We have an argument in next command line parameter
                _getOptData.optionArgument = argv[_getOptData.optionIndex++];
            _getOptData.nextChar = nullptr;
        }
    }
    return c;
}

int CommandLineParser::GetOpt(size_t argCount, const OSAL::Char ** argv, const OSAL::String & optionString,
                              size_t & optionIndex)
{
    if (argCount < 1)
        return -1;

    const OSAL::Char * optionStringPtr = optionString.c_str();
    bool printErrors = _getOptData.printErrors;
    _getOptData.optionArgument = nullptr;

    if (('-' == *optionStringPtr) || ('+' == *optionStringPtr))
        optionStringPtr++;

    if (*optionStringPtr == ':')
        printErrors = false;
    if ((nullptr == _getOptData.nextChar) || ('\0' == *_getOptData.nextChar))
    {
        _getOptData.lastNonOption = std::min(_getOptData.lastNonOption, _getOptData.optionIndex);
        _getOptData.firstNonOption = std::min(_getOptData.firstNonOption, _getOptData.optionIndex);
        if (CommandLineParser::Ordering::Permute == _getOptData.ordering)
        {
            if ((_getOptData.firstNonOption != _getOptData.lastNonOption) &&
                (_getOptData.lastNonOption != _getOptData.optionIndex))
                ExchangeOption(argv);
            else if (_getOptData.lastNonOption != _getOptData.optionIndex)
                _getOptData.firstNonOption = _getOptData.optionIndex;
            // Skip non-_options and empty _options
            while (AtNonOption(argCount, argv))
                _getOptData.optionIndex++;
            _getOptData.lastNonOption = _getOptData.optionIndex;
        }
        if ((_getOptData.optionIndex < argCount ) && !OSAL::Strings::strcmp(argv[_getOptData.optionIndex], _("--")))
        {
            // We are apparently using stdin as input?
            _getOptData.optionIndex++;
            if ((_getOptData.firstNonOption != _getOptData.lastNonOption) &&
                (_getOptData.lastNonOption != _getOptData.optionIndex))
                ExchangeOption(argv);
            else if (_getOptData.firstNonOption == _getOptData.lastNonOption)
                _getOptData.firstNonOption = _getOptData.optionIndex;
            _getOptData.lastNonOption = _getOptData.optionIndex = argCount;
        }
        if (_getOptData.optionIndex == argCount)
        {
            // Point to first non-option
            if (_getOptData.firstNonOption != _getOptData.lastNonOption)
                _getOptData.optionIndex = _getOptData.firstNonOption;
            return -1;
        }
        if (AtNonOption(argCount, argv))
        {
            if (_getOptData.ordering == CommandLineParser::Ordering::RequireOrder)
                // There ar no more options
                return -1;
            _getOptData.optionArgument = argv[_getOptData.optionIndex++];
            return 1;
        }
        _getOptData.nextChar = (argv[_getOptData.optionIndex] + 1 +
                                (('-' == argv[_getOptData.optionIndex][1])? 1 : 0));
    }
    if (AtLongOption(argCount, argv))
    {
        return HandleLongOption(argCount, argv, optionStringPtr, optionIndex, printErrors);
    }
    return HandleShortOption(argCount, argv, optionStringPtr, printErrors);
}

void CommandLineParser::ShowHelp(const OSAL::String & applicationName)
{
    _console << fgcolor(OSAL::ConsoleColor::Yellow) << GetHelp(applicationName)
             << endl;
    _console << fgcolor(OSAL::ConsoleColor::Default);
}

void CommandLineParser::AddOption(const CommandLineOption::Ptr option)
{
    _mainGroup->AddOption(option);
}

size_t CommandLineParser::MatchShortOption(OSAL::Char name) const
{
    for (size_t i = 0; i < _options.size(); i++)
    {
        if (_options[i]->ShortName() == name)
        {
            return i;
        }
    }
    return numeric_limits<size_t>::max();
}

size_t CommandLineParser::MatchLongOption(const OSAL::String & name) const
{
    return MatchLongOption(name.c_str(), name.length());
}

size_t CommandLineParser::MatchLongOption(const OSAL::Char * name, size_t nameLength) const
{
    for (size_t i = 0; i < _options.size(); i++)
    {
        // find matching long option
        if (_options[i]->LongName() == OSAL::String(name, nameLength))
        {
            // exact match
            return i;
        }
        // If this is a known short option, don't allow a partial match of a single character.
        if ((nameLength == 1) && (*name == _options[i]->ShortName()))
            return i;
    }
    return numeric_limits<size_t>::max();
}

void CommandLineParser::SelectOption(CommandLineOption::Ptr option, const OSAL::Char * value)
{
    option->SetOptionFound();
    OSAL::String argument = (value != nullptr) ? value : _("");
    argument = OSAL::Trim(argument, _("\""));
    switch (option->ArgType())
    {
        case CommandLineArgumentType::NoArgument:
            break;
        case CommandLineArgumentType::OptionalArgument:
            if (value != nullptr)
                option->Argument(argument);
            break;
        case CommandLineArgumentType::RequiredArgument:
            option->Argument(argument);
            break;
    }
    OnParseOption(option);
}

void CommandLineParser::AddNonOption(const OSAL::Char * value)
{
    _nonOptions.push_back(OSAL::String(OSAL::Trim(value, _("\""))));
    OnParseNonOption(value);
}

void CommandLineParser::AddGroup(const CommandLineOptionGroup::Ptr group)
{
    _groups.push_back(group);
}

void CommandLineParser::MainGroup(const CommandLineOptionGroup::Ptr group)
{
    if (!IsInGroups(group))
    {
        AddGroup(group);
    }
    _mainGroup = group;
}

bool CommandLineParser::IsInGroups(const CommandLineOptionGroup::Ptr group) const
{
    for (auto groupFound : _groups)
    {
        if (groupFound == group)
            return true;
    }
    return false;
}

OSAL::String CommandLineParser::GetHelp(const OSAL::String & applicationName, bool mainHelp /* = true*/) const
{
    return GetHelp(applicationName, mainHelp, CommandLineOptionGroup::Ptr());
}

OSAL::String CommandLineParser::GetHelp(const OSAL::String & applicationName, bool UNUSED(mainHelp),
                                        const CommandLineOptionGroup::Ptr group) const
{
    OSAL::String strippedPath = OSAL::Path::LastPartOfPath(applicationName);
    const int indentOptions = 2;
    const int indentDescription = 25;
    basic_ostringstream<OSAL::Char> stream;
    stream << _("Usage:") << endl;
    stream << _name << _(": ") << _description << endl << endl;
    stream << OSAL::String(indentOptions, ' ') << strippedPath << _(" [OPTION...]") << endl << endl;
    for (auto foundGroup : _groups)
    {
        if ((group != nullptr) && (group != foundGroup))
            continue;
        if (_options.size() > 0)
        {
            for (auto option : foundGroup->Options())
            {
                OSAL::String optionsText = (option->ShortName() != 0)
                                           ? OSAL::String(_("-")) + option->ShortName() + _(", ")
                                           : _("");
                optionsText += _("--") + option->LongName();
                if (option->ArgType() == CommandLineArgumentType::OptionalArgument)
                {
                    optionsText += _(" [argument]");
                } else if (option->ArgType() == CommandLineArgumentType::RequiredArgument)
                {
                    optionsText += _(" <argument>");
                }
                stream << OSAL::String(indentOptions, ' ') << optionsText;
                if (optionsText.length() < indentDescription)
                    stream << OSAL::String(indentDescription - optionsText.length(), _(' '));
                stream << _(" ") << option->Description() << endl;
            }
            stream << endl;
        }
    }
    return stream.str();
}

void CommandLineParser::FillOptionsList()
{
    for (auto group : _groups)
    {
        for (auto option : group->Options())
        {
            _options.push_back(option);
        }
    }
}

void CommandLineParser::SetupStandardGroups()
{
    CommandLineOptionGroup::Ptr mainGroup(new CommandLineOptionGroup(_name, _description));
    AddGroup(mainGroup);

    CommandLineOptionGroup::Ptr helpGroup(new CommandLineOptionGroup(HelpOptionsGroupName));
    AddGroup(helpGroup);
    helpGroup->AddSwitchWithVariable(_("help"), _showHelp, true, _("Show help options"));

    MainGroup(mainGroup);
}

