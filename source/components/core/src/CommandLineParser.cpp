#include "core/CommandLineParser.h"

#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include "osal/osal.h"
#include "osal/path.h"

using namespace Core;

const OSAL::String CommandLineParser::DefaultMainOptionsGroupName = "Main";

CommandLineParser::CommandLineParser(OSAL::Console & console,
                                     const OSAL::String & name,
                                     const OSAL::String & description)
    : _console(console)
    , _autoHelp(true)
    , _showHelp(false)
    , _name(name)
    , _description(description)
    , _nonOptions()
    , _options()
    , _getoptData()
{

}

CommandLineParser::~CommandLineParser()
{
    _nonOptions.clear();
    _options.clear();
}

void CommandLineParser::AddOptionNoArgument(const OSAL::String & longName, int & variable, int value,
                                            const OSAL::String & description)
{
    AddOption(std::make_shared<CommandLineOption>(longName, description, variable, value));
//    AddOption(std::make_shared<CommandLineOptionNoArgumentWithVariable<int>>(longName, variable, value, description));
}

//void CommandLineParser::AddOptionNoArgument(const OSAL::String longName, bool &variable, bool value,
//                                            const OSAL::String & description)
//{
//    AddOption(std::make_shared<CommandLineOption>(longName, description, variable, value));
////    AddOption(std::make_shared<CommandLineOptionNoArgumentWithVariable<bool>>(longName, variable, value, description));
//}

void CommandLineParser::AddOptionNoArgument(OSAL::Char shortName, const OSAL::String & longName,
                                            const OSAL::String & description)
{
    AddOption(std::make_shared<CommandLineOption>(longName, shortName, description));
//    AddOption(std::make_shared<CommandLineOptionNoArgument>(shortName, longName, description));
}

void CommandLineParser::AddOptionOptionalArgument(OSAL::Char shortName, const OSAL::String & longName,
                                                  const OSAL::String & description, OSAL::String & textVariable)
{
    AddOption(std::make_shared<CommandLineOption>(longName, shortName, description, textVariable,
                                                  CommandLineArgumentType::OptionalArgument));
}

void CommandLineParser::AddOptionOptionalArgument(const OSAL::String & longName,
                                                  const OSAL::String & description, OSAL::String & textVariable)
{
    AddOption(std::make_shared<CommandLineOption>(longName, _('\0'), description, textVariable,
                                                  CommandLineArgumentType::OptionalArgument));
}

void CommandLineParser::AddOptionRequiredArgument(OSAL::Char shortName, OSAL::String longName,
                                                  const OSAL::String & description, OSAL::String & textVariable)
{
    AddOption(std::make_shared<CommandLineOption>(longName, shortName, description, textVariable,
                                                  CommandLineArgumentType::RequiredArgument));
}

void CommandLineParser::AddOptionRequiredArgument(OSAL::String longName,
                                                  const OSAL::String & description, OSAL::String & textVariable)
{
    AddOption(std::make_shared<CommandLineOption>(longName, _('\0'), description, textVariable,
                                                  CommandLineArgumentType::RequiredArgument));
}

//void CommandLineParser::AddOptionWithArgument(OSAL::Char shortName, OSAL::String longName, OSAL::String &variable,
//                                              const OSAL::String & description)
//{
//
//}

OSAL::String CommandLineParser::GetNonOption(size_t index) const
{
    if (index < _nonOptions.size())
        return _nonOptions[index];
    return "";
}

bool CommandLineParser::HaveOption(OSAL::Char shortName) const
{
    size_t index = MatchShortOption(shortName);
    return (index < std::numeric_limits<size_t>::max());
}

bool CommandLineParser::HaveOption(OSAL::String longName) const
{
    size_t index = MatchLongOption(longName.c_str(), longName.length());
    return (index < std::numeric_limits<size_t>::max());
}

bool CommandLineParser::FoundOption(OSAL::Char shortName) const
{
    size_t index = MatchShortOption(shortName);
    return (index < std::numeric_limits<size_t>::max()) && _options[index]->FoundOption();
}

bool CommandLineParser::FoundOption(OSAL::String longName) const
{
    size_t index = MatchLongOption(longName.c_str(), longName.length());
    return (index < std::numeric_limits<size_t>::max()) && _options[index]->FoundOption();
}

OSAL::String CommandLineParser::GetOption(OSAL::Char shortName) const
{
    size_t index = MatchShortOption(shortName);
    return (index < std::numeric_limits<size_t>::max())
           ? _options[index]->Argument()
           : "";
}

OSAL::String CommandLineParser::GetOption(OSAL::String longName) const
{
    size_t index = MatchLongOption(longName.c_str(), longName.length());
    return (index < std::numeric_limits<size_t>::max())
           ? _options[index]->Argument()
           : "";
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
    catch (const std::exception & e)
    {
        _console << fgcolor(OSAL::ConsoleColor::Red) << _("Exception: ") << e.what() << std::endl;
        if (_autoHelp)
        {
            ShowHelp(argv[0]);
        }
        return false;
    }
    return true;
}

OSAL::String CommandLineParser::GetHelp(const OSAL::String & applicationName) const
{
    OSAL::String strippedPath = OSAL::Path::LastPartOfPath(applicationName);
    const int indentOptions = 2;
    const int indentDescription = 18;
    std::ostringstream stream;
    stream << _("Usage:") << std::endl;
    stream << _name << _(": ") << _description << std::endl << std::endl;
    stream << OSAL::String(indentOptions, ' ') << strippedPath << _(" [OPTION...]") << std::endl << std::endl;
    if (_options.size() > 0)
    {
        for (auto option : _options)
        {
            OSAL::String optionsText = (option->ShortName() != 0)
                                 ? OSAL::String("-") + option->ShortName() + _(", ")
                                 : _("");
            optionsText += _("--") + option->LongName();
            stream << OSAL::String(indentOptions, ' ') << optionsText;
            if (optionsText.length() < indentDescription)
                stream << OSAL::String(indentDescription - optionsText.length(), _(' '));
            stream << _(" ") << option->Description() << std::endl;
        }
        stream << std::endl;
    }
    return stream.str();
}

bool CommandLineParser::InternalParse(int argc, const OSAL::Char * argv[])
{
    size_t numOptions = _options.size();
    std::vector<std::string> strings(numOptions);
    std::vector<option_a> options;
    OSAL::String optionString;
    for (auto opt : _options)
    {
        option_a long_option;
        long_option.name = opt->LongName().c_str();
        long_option.argumentType = opt->ArgType();
        long_option.flag = opt->Variable();
        long_option.val = !opt->Variable()
                          ? static_cast<int>(opt->ShortName())
                          : opt->Value();
        options.push_back(long_option);
        if (!opt->Variable())
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
    while (result != -1)
    {
        result = GetOpt(argc, argv, optionString, &(options[0]), optionIndex);
        if (result == 0)
        {
            size_t index = optionIndex;
            CommandLineOptionPtr option = _options[index];
            if (options[optionIndex].flag != 0)
            {
                SelectOption(option, nullptr);
                _console << "Option " << options[optionIndex].name << " automatically set" << std::endl;
            } else
            {
                if (_getoptData.optarg && (*_getoptData.optarg == '='))
                    _getoptData.optarg++;
                SelectOption(option, _getoptData.optarg);
                _console << "Option " << options[optionIndex].name;
                if (_getoptData.optarg)
                    _console << " with arg " << _getoptData.optarg;
                _console << std::endl;
            }
        } else if (result == '?')
        {
            break;
        } else if (result != -1)
        {
            size_t index = MatchShortOption(static_cast<char>(result));
            if (index < std::numeric_limits<size_t>::max())
            {
                CommandLineOptionPtr option = _options[index];
                switch (option->ArgType())
                {
                    case CommandLineArgumentType::NoArgument:
                        SelectOption(option, nullptr);
                        _console << "Option " << option->LongName() << " found" << std::endl;
                        break;
                    case CommandLineArgumentType::OptionalArgument:
                        if (_getoptData.optarg && (*_getoptData.optarg == '='))
                            _getoptData.optarg++;
                        SelectOption(option, _getoptData.optarg);
                        _console << "Option " << option->LongName() << " found";
                        if (_getoptData.optarg)
                            _console << " with arg " << _getoptData.optarg;
                        else
                            _console << " with no argument";
                        _console << std::endl;
                        break;
                    case CommandLineArgumentType::RequiredArgument:
                        if (_getoptData.optarg && (*_getoptData.optarg == '='))
                            _getoptData.optarg++;
                        SelectOption(option, _getoptData.optarg);
                        _console << "Option " << option->LongName() << " found";
                        if (_getoptData.optarg)
                            _console << " with arg " << _getoptData.optarg;
                        else
                            _console << " without required argument!";
                        _console << std::endl;
                        break;
                    default:
                        _console << "Invalid result " << option->ArgType() << " for argument type found" << std::endl;
                        break;
                }
            }
        }

    }

    while (_getoptData.optind < argc)
    {
        AddNonOption(argv[_getoptData.optind]);
        _console << "Extra argument " << argv[_getoptData.optind] << std::endl;
        _getoptData.optind++;
    }

    return (result != '?');
}

int CommandLineParser::GetOpt(int argc, const OSAL::Char ** argv, const OSAL::String & optionString, const option_a *long_options,
                              size_t & optionIndex)
{
    int result;
    result = GetOptInternal(argc, argv, optionString.c_str(), long_options, optionIndex);
    return result;
}

void CommandLineParser::ExchangeOption(const OSAL::Char ** argv)
{
    int bottom = _getoptData.__first_nonopt;
    int middle = _getoptData.__last_nonopt;
    int top = _getoptData.optind;
    const OSAL::Char * tem;
    while (top > middle && middle > bottom)
    {
        if (top - middle > middle - bottom)
        {
            int len = middle - bottom;
            register int i;
            for (i = 0; i < len; i++)
            {
                tem = argv[bottom + i];
                argv[bottom + i] = argv[top - (middle - bottom) + i];
                argv[top - (middle - bottom) + i] = tem;
            }
            top -= len;
        }
        else
        {
            int len = top - middle;
            register int i;
            for (i = 0; i < len; i++)
            {
                tem = argv[bottom + i];
                argv[bottom + i] = argv[middle + i];
                argv[middle + i] = tem;
            }
            bottom += len;
        }
    }
    _getoptData.__first_nonopt += (_getoptData.optind - _getoptData.__last_nonopt);
    _getoptData.__last_nonopt = _getoptData.optind;
}

const OSAL::Char * CommandLineParser::Initialize(const OSAL::Char *optstring)
{
    _getoptData.__first_nonopt = _getoptData.__last_nonopt = _getoptData.optind;
    _getoptData.__nextchar = nullptr;
    if (optstring[0] == '-')
    {
        _getoptData.__ordering = CommandLineParser::ENUM_ORDERING::RETURN_IN_ORDER;
        ++optstring;
    }
    else if (optstring[0] == '+')
    {
        _getoptData.__ordering = CommandLineParser::ENUM_ORDERING::REQUIRE_ORDER;
        ++optstring;
    }
    else
        _getoptData.__ordering = CommandLineParser::ENUM_ORDERING::PERMUTE;
    return optstring;
}

int CommandLineParser::GetOptInternal(int argc, const OSAL::Char ** argv, const OSAL::String &optionString,
                                      const CommandLineParser::option_a *longopts,
                                      size_t &longind)
{
    const OSAL::Char * optstring = optionString.c_str();
    int print_errors = _getoptData.opterr;
    if (argc < 1)
        return -1;
    _getoptData.optarg = nullptr;
    if (_getoptData.optind == 0 || !_getoptData.initialized)
    {
        if (_getoptData.optind == 0)
            _getoptData.optind = 1;
        optstring = Initialize(optstring);
        _getoptData.initialized = true;
    }
    else if (optstring[0] == '-' || optstring[0] == '+')
        optstring++;
    if (optstring[0] == ':')
        print_errors = 0;
    if (_getoptData.__nextchar == nullptr || *_getoptData.__nextchar == '\0')
    {
        if (_getoptData.__last_nonopt > _getoptData.optind)
            _getoptData.__last_nonopt = _getoptData.optind;
        if (_getoptData.__first_nonopt > _getoptData.optind)
            _getoptData.__first_nonopt = _getoptData.optind;
        if (_getoptData.__ordering == CommandLineParser::ENUM_ORDERING::PERMUTE)
        {
            if (_getoptData.__first_nonopt != _getoptData.__last_nonopt && _getoptData.__last_nonopt != _getoptData.optind)
                ExchangeOption(argv);
            else if (_getoptData.__last_nonopt != _getoptData.optind)
                _getoptData.__first_nonopt = _getoptData.optind;
            while (_getoptData.optind < argc && (argv[_getoptData.optind][0] != '-' || argv[_getoptData.optind][1] == '\0'))
                _getoptData.optind++;
            _getoptData.__last_nonopt = _getoptData.optind;
        }
        if (_getoptData.optind != argc && !strcmp(argv[_getoptData.optind], "--"))
        {
            _getoptData.optind++;
            if (_getoptData.__first_nonopt != _getoptData.__last_nonopt && _getoptData.__last_nonopt != _getoptData.optind)
                ExchangeOption(argv);
            else if (_getoptData.__first_nonopt == _getoptData.__last_nonopt)
                _getoptData.__first_nonopt = _getoptData.optind;
            _getoptData.__last_nonopt = argc;
            _getoptData.optind = argc;
        }
        if (_getoptData.optind == argc)
        {
            if (_getoptData.__first_nonopt != _getoptData.__last_nonopt)
                _getoptData.optind = _getoptData.__first_nonopt;
            return -1;
        }
        if ((argv[_getoptData.optind][0] != '-' || argv[_getoptData.optind][1] == '\0'))
        {
            if (_getoptData.__ordering == CommandLineParser::ENUM_ORDERING::REQUIRE_ORDER)
                return -1;
            _getoptData.optarg = argv[_getoptData.optind++];
            return 1;
        }
        _getoptData.__nextchar = (argv[_getoptData.optind] + 1 + (longopts != nullptr && argv[_getoptData.optind][1] == '-'));
    }
    if ((longopts != nullptr) && (argv[_getoptData.optind][1] == '-'))
    {
        const OSAL::Char *nameend;
        unsigned int namelen;
        const CommandLineParser::option_a *p;
        const CommandLineParser::option_a *pfound = nullptr;
        struct option_list
        {
            const CommandLineParser::option_a *p;
            struct option_list *next;
        } *ambig_list = nullptr;
        int exact = 0;
        ssize_t indfound = -1;
        size_t option_index;
        for (nameend = _getoptData.__nextchar; *nameend && *nameend != '='; nameend++);
        namelen = (unsigned int)(nameend - _getoptData.__nextchar);
        for (p = longopts, option_index = 0; p->name; p++, option_index++)
            if (!strncmp(p->name, _getoptData.__nextchar, namelen))
            {
                if (namelen == (unsigned int)strlen(p->name))
                {
                    pfound = p;
                    indfound = option_index;
                    exact = 1;
                    break;
                }
                else if (pfound == nullptr)
                {
                    pfound = p;
                    indfound = option_index;
                }
                else if (pfound->argumentType != p->argumentType || pfound->flag != p->flag || pfound->val != p->val)
                {
                    struct option_list *newp = (struct option_list*)alloca(sizeof(*newp));
                    newp->p = p;
                    newp->next = ambig_list;
                    ambig_list = newp;
                }
            }
        if (ambig_list != nullptr && !exact)
        {
            if (print_errors)
            {
                struct option_list first;
                first.p = pfound;
                first.next = ambig_list;
                ambig_list = &first;
                fprintf (stderr, "%s: option '%s' is ambiguous; possibilities:", argv[0], argv[_getoptData.optind]);
                do
                {
                    fprintf (stderr, " '--%s'", ambig_list->p->name);
                    ambig_list = ambig_list->next;
                }
                while (ambig_list != nullptr);
                fputc ('\n', stderr);
            }
            _getoptData.__nextchar += strlen(_getoptData.__nextchar);
            _getoptData.optind++;
            _getoptData.optopt = 0;
            return '?';
        }
        if (pfound != nullptr)
        {
            option_index = indfound;
            _getoptData.optind++;
            if (*nameend)
            {
                if (pfound->argumentType != CommandLineArgumentType::NoArgument)
                    _getoptData.optarg = nameend + 1;
                else
                {
                    if (print_errors)
                    {
                        if (argv[_getoptData.optind - 1][1] == '-')
                        {
                            fprintf(stderr, "%s: option '--%s' doesn't allow an argument\n",argv[0], pfound->name);
                        }
                        else
                        {
                            fprintf(stderr, "%s: option '%c%s' doesn't allow an argument\n",argv[0], argv[_getoptData.optind - 1][0],pfound->name);
                        }
                    }
                    _getoptData.__nextchar += strlen(_getoptData.__nextchar);
                    _getoptData.optopt = pfound->val;
                    return '?';
                }
            }
            else if (pfound->argumentType == CommandLineArgumentType::RequiredArgument)
            {
                if (_getoptData.optind < argc)
                    _getoptData.optarg = argv[_getoptData.optind++];
                else
                {
                    if (print_errors)
                    {
                        fprintf(stderr,"%s: option '--%s' requires an argument\n",argv[0], pfound->name);
                    }
                    _getoptData.__nextchar += strlen(_getoptData.__nextchar);
                    _getoptData.optopt = pfound->val;
                    return optstring[0] == ':' ? ':' : '?';
                }
            }
            _getoptData.__nextchar += strlen(_getoptData.__nextchar);
            longind = option_index;
            if (pfound->flag)
            {
                *(pfound->flag) = pfound->val;
                return 0;
            }
            return pfound->val;
        }
        if (print_errors)
        {
            if (argv[_getoptData.optind][1] == '-')
            {
                fprintf(stderr, "%s: unrecognized option '--%s'\n",argv[0], _getoptData.__nextchar);
            }
            else
            {
                fprintf(stderr, "%s: unrecognized option '%c%s'\n",argv[0], argv[_getoptData.optind][0], _getoptData.__nextchar);
            }
        }
        _getoptData.__nextchar = (OSAL::Char *)"";
        _getoptData.optind++;
        _getoptData.optopt = 0;
        return '?';
    }
    {
        OSAL::Char c = *_getoptData.__nextchar++;
        OSAL::Char *temp = (OSAL::Char*)strchr(optstring, c);
        if (*_getoptData.__nextchar == '\0')
            ++_getoptData.optind;
        if (temp == nullptr || c == ':' || c == ';')
        {
            if (print_errors)
            {
                fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], c);
            }
            _getoptData.optopt = c;
            return '?';
        }
        if (temp[0] == 'W' && temp[1] == ';')
        {
            const OSAL::Char *nameend;
            const CommandLineParser::option_a *p;
            const CommandLineParser::option_a *pfound = nullptr;
            int exact = 0;
            int ambig = 0;
            int indfound = 0;
            size_t option_index;
            if (longopts == nullptr)
                goto no_longs;
            if (*_getoptData.__nextchar != '\0')
            {
                _getoptData.optarg = _getoptData.__nextchar;
                _getoptData.optind++;
            }
            else if (_getoptData.optind == argc)
            {
                if (print_errors)
                {
                    fprintf(stderr,"%s: option requires an argument -- '%c'\n",argv[0], c);
                }
                _getoptData.optopt = c;
                if (optstring[0] == ':')
                    c = ':';
                else
                    c = '?';
                return c;
            }
            else
                _getoptData.optarg = argv[_getoptData.optind++];
            for (_getoptData.__nextchar = nameend = _getoptData.optarg; *nameend && *nameend != '='; nameend++);
            for (p = longopts, option_index = 0; p->name; p++, option_index++)
                if (!strncmp(p->name, _getoptData.__nextchar, nameend - _getoptData.__nextchar))
                {
                    if ((unsigned int) (nameend - _getoptData.__nextchar) == strlen(p->name))
                    {
                        pfound = p;
                        indfound = option_index;
                        exact = 1;
                        break;
                    }
                    else if (pfound == nullptr)
                    {
                        pfound = p;
                        indfound = option_index;
                    }
                    else if (pfound->argumentType != p->argumentType || pfound->flag != p->flag || pfound->val != p->val)
                        ambig = 1;
                }
            if (ambig && !exact)
            {
                if (print_errors)
                {
                    fprintf(stderr, "%s: option '-W %s' is ambiguous\n",argv[0], _getoptData.optarg);
                }
                _getoptData.__nextchar += strlen(_getoptData.__nextchar);
                _getoptData.optind++;
                return '?';
            }
            if (pfound != nullptr)
            {
                option_index = indfound;
                if (*nameend)
                {
                    if (pfound->argumentType != CommandLineArgumentType::NoArgument)
                        _getoptData.optarg = nameend + 1;
                    else
                    {
                        if (print_errors)
                        {
                            fprintf(stderr, "%s: option '-W %s' doesn't allow an argument\n",argv[0], pfound->name);
                        }
                        _getoptData.__nextchar += strlen(_getoptData.__nextchar);
                        return '?';
                    }
                }
                else if (pfound->argumentType == CommandLineArgumentType::RequiredArgument)
                {
                    if (_getoptData.optind < argc)
                        _getoptData.optarg = argv[_getoptData.optind++];
                    else
                    {
                        if (print_errors)
                        {
                            fprintf(stderr, "%s: option '-W %s' requires an argument\n",argv[0], pfound->name);
                        }
                        _getoptData.__nextchar += strlen(_getoptData.__nextchar);
                        return optstring[0] == ':' ? ':' : '?';
                    }
                }
                else
                    _getoptData.optarg = nullptr;
                _getoptData.__nextchar += strlen(_getoptData.__nextchar);
                longind = option_index;
                if (pfound->flag)
                {
                    *(pfound->flag) = pfound->val;
                    return 0;
                }
                return pfound->val;
            }
            no_longs:
            _getoptData.__nextchar = nullptr;
            return 'W';
        }
        if (temp[1] == ':')
        {
            if (temp[2] == ':')
            {
                if (*_getoptData.__nextchar != '\0')
                {
                    _getoptData.optarg = _getoptData.__nextchar;
                    _getoptData.optind++;
                }
                else
                    _getoptData.optarg = nullptr;
                _getoptData.__nextchar = nullptr;
            }
            else
            {
                if (*_getoptData.__nextchar != '\0')
                {
                    _getoptData.optarg = _getoptData.__nextchar;
                    _getoptData.optind++;
                }
                else if (_getoptData.optind == argc)
                {
                    if (print_errors)
                    {
                        fprintf(stderr,"%s: option requires an argument -- '%c'\n",argv[0], c);
                    }
                    _getoptData.optopt = c;
                    if (optstring[0] == ':')
                        c = ':';
                    else
                        c = '?';
                }
                else
                    _getoptData.optarg = argv[_getoptData.optind++];
                _getoptData.__nextchar = nullptr;
            }
        }
        return c;
    }
}

void CommandLineParser::ShowHelp(const OSAL::String & applicationName)
{
    _console << fgcolor(OSAL::ConsoleColor::Yellow) << GetHelp(applicationName)
             << std::endl;
    _console << fgcolor(OSAL::ConsoleColor::Default);
}

void CommandLineParser::AddOption(const CommandLineOptionPtr option)
{
    _options.push_back(option);
}

size_t CommandLineParser::MatchShortOption(OSAL::Char name) const
{
    for (size_t i = 0; i < _options.size(); i++)
    {
        if (_options[i]->ShortName() == name)
        {
            return (int)i;
        }
    }
    return std::numeric_limits<size_t>::max();
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
            return static_cast<int>(i);
        }
        // If this is a known short option, don't allow a partial match of a single character.
        if ((nameLength == 1) && (*name == _options[i]->ShortName()))
            return static_cast<int>(i);
    }
    return std::numeric_limits<size_t>::max();
}

void CommandLineParser::SelectOption(CommandLineOptionPtr option, const OSAL::Char * value)
{
    option->FoundOption(true);
    OSAL::String argument = (value != nullptr) ? value : "";
    argument = OSAL::Trim(argument, "\"");
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
    _nonOptions.push_back(OSAL::String(OSAL::Trim(value, "\"")));
    OnParseNonOption(value);
}
