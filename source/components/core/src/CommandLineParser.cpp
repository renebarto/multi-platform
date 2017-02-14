#include "core/CommandLineParser.h"

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include "osal/osal.h"
#include "osal/path.h"

using namespace Core;

static int _optind = 1;
static int _opterr = 1;
static int _optopt = '?';
enum ENUM_ORDERING { REQUIRE_ORDER, PERMUTE, RETURN_IN_ORDER };

struct option_a
{
    const char* name;
    int has_arg;
    int *flag;
    int val;
};
struct _getopt_data_a
{
    int optind;
    int opterr;
    int optopt;
    char *optarg;
    int __initialized;
    char *__nextchar;
    enum ENUM_ORDERING __ordering;
    int __posixly_correct;
    int __first_nonopt;
    int __last_nonopt;
};
static struct _getopt_data_a getopt_data_a;
static char *optarg_a;

static void exchange_a(char **argv, struct _getopt_data_a *d)
{
    int bottom = d->__first_nonopt;
    int middle = d->__last_nonopt;
    int top = d->optind;
    char *tem;
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
    d->__first_nonopt += (d->optind - d->__last_nonopt);
    d->__last_nonopt = d->optind;
}
static const char *_getopt_initialize_a (const char *optstring, struct _getopt_data_a *d, int posixly_correct)
{
    d->__first_nonopt = d->__last_nonopt = d->optind;
    d->__nextchar = NULL;
    d->__posixly_correct = posixly_correct | !!getenv("POSIXLY_CORRECT");
    if (optstring[0] == '-')
    {
        d->__ordering = RETURN_IN_ORDER;
        ++optstring;
    }
    else if (optstring[0] == '+')
    {
        d->__ordering = REQUIRE_ORDER;
        ++optstring;
    }
    else if (d->__posixly_correct)
        d->__ordering = REQUIRE_ORDER;
    else
        d->__ordering = PERMUTE;
    return optstring;
}
int _getopt_internal_r_a (int argc, char *const *argv, const char *optstring, const struct option_a *longopts, int *longind, int long_only, struct _getopt_data_a *d, int posixly_correct)
{
    int print_errors = d->opterr;
    if (argc < 1)
        return -1;
    d->optarg = NULL;
    if (d->optind == 0 || !d->__initialized)
    {
        if (d->optind == 0)
            d->optind = 1;
        optstring = _getopt_initialize_a (optstring, d, posixly_correct);
        d->__initialized = 1;
    }
    else if (optstring[0] == '-' || optstring[0] == '+')
        optstring++;
    if (optstring[0] == ':')
        print_errors = 0;
    if (d->__nextchar == NULL || *d->__nextchar == '\0')
    {
        if (d->__last_nonopt > d->optind)
            d->__last_nonopt = d->optind;
        if (d->__first_nonopt > d->optind)
            d->__first_nonopt = d->optind;
        if (d->__ordering == PERMUTE)
        {
            if (d->__first_nonopt != d->__last_nonopt && d->__last_nonopt != d->optind)
                exchange_a ((char **) argv, d);
            else if (d->__last_nonopt != d->optind)
                d->__first_nonopt = d->optind;
            while (d->optind < argc && (argv[d->optind][0] != '-' || argv[d->optind][1] == '\0'))
                d->optind++;
            d->__last_nonopt = d->optind;
        }
        if (d->optind != argc && !strcmp(argv[d->optind], "--"))
        {
            d->optind++;
            if (d->__first_nonopt != d->__last_nonopt && d->__last_nonopt != d->optind)
                exchange_a((char **) argv, d);
            else if (d->__first_nonopt == d->__last_nonopt)
                d->__first_nonopt = d->optind;
            d->__last_nonopt = argc;
            d->optind = argc;
        }
        if (d->optind == argc)
        {
            if (d->__first_nonopt != d->__last_nonopt)
                d->optind = d->__first_nonopt;
            return -1;
        }
        if ((argv[d->optind][0] != '-' || argv[d->optind][1] == '\0'))
        {
            if (d->__ordering == REQUIRE_ORDER)
                return -1;
            d->optarg = argv[d->optind++];
            return 1;
        }
        d->__nextchar = (argv[d->optind] + 1 + (longopts != NULL && argv[d->optind][1] == '-'));
    }
    if (longopts != NULL && (argv[d->optind][1] == '-' || (long_only && (argv[d->optind][2] || !strchr(optstring, argv[d->optind][1])))))
    {
        char *nameend;
        unsigned int namelen;
        const struct option_a *p;
        const struct option_a *pfound = NULL;
        struct option_list
        {
            const struct option_a *p;
            struct option_list *next;
        } *ambig_list = NULL;
        int exact = 0;
        int indfound = -1;
        int option_index;
        for (nameend = d->__nextchar; *nameend && *nameend != '='; nameend++);
        namelen = (unsigned int)(nameend - d->__nextchar);
        for (p = longopts, option_index = 0; p->name; p++, option_index++)
            if (!strncmp(p->name, d->__nextchar, namelen))
            {
                if (namelen == (unsigned int)strlen(p->name))
                {
                    pfound = p;
                    indfound = option_index;
                    exact = 1;
                    break;
                }
                else if (pfound == NULL)
                {
                    pfound = p;
                    indfound = option_index;
                }
                else if (long_only || pfound->has_arg != p->has_arg || pfound->flag != p->flag || pfound->val != p->val)
                {
                    struct option_list *newp = (struct option_list*)alloca(sizeof(*newp));
                    newp->p = p;
                    newp->next = ambig_list;
                    ambig_list = newp;
                }
            }
        if (ambig_list != NULL && !exact)
        {
            if (print_errors)
            {
                struct option_list first;
                first.p = pfound;
                first.next = ambig_list;
                ambig_list = &first;
                fprintf (stderr, "%s: option '%s' is ambiguous; possibilities:", argv[0], argv[d->optind]);
                do
                {
                    fprintf (stderr, " '--%s'", ambig_list->p->name);
                    ambig_list = ambig_list->next;
                }
                while (ambig_list != NULL);
                fputc ('\n', stderr);
            }
            d->__nextchar += strlen(d->__nextchar);
            d->optind++;
            d->optopt = 0;
            return '?';
        }
        if (pfound != NULL)
        {
            option_index = indfound;
            d->optind++;
            if (*nameend)
            {
                if (pfound->has_arg)
                    d->optarg = nameend + 1;
                else
                {
                    if (print_errors)
                    {
                        if (argv[d->optind - 1][1] == '-')
                        {
                            fprintf(stderr, "%s: option '--%s' doesn't allow an argument\n",argv[0], pfound->name);
                        }
                        else
                        {
                            fprintf(stderr, "%s: option '%c%s' doesn't allow an argument\n",argv[0], argv[d->optind - 1][0],pfound->name);
                        }
                    }
                    d->__nextchar += strlen(d->__nextchar);
                    d->optopt = pfound->val;
                    return '?';
                }
            }
            else if (pfound->has_arg == 1)
            {
                if (d->optind < argc)
                    d->optarg = argv[d->optind++];
                else
                {
                    if (print_errors)
                    {
                        fprintf(stderr,"%s: option '--%s' requires an argument\n",argv[0], pfound->name);
                    }
                    d->__nextchar += strlen(d->__nextchar);
                    d->optopt = pfound->val;
                    return optstring[0] == ':' ? ':' : '?';
                }
            }
            d->__nextchar += strlen(d->__nextchar);
            if (longind != NULL)
                *longind = option_index;
            if (pfound->flag)
            {
                *(pfound->flag) = pfound->val;
                return 0;
            }
            return pfound->val;
        }
        if (!long_only || argv[d->optind][1] == '-' || strchr(optstring, *d->__nextchar) == NULL)
        {
            if (print_errors)
            {
                if (argv[d->optind][1] == '-')
                {
                    fprintf(stderr, "%s: unrecognized option '--%s'\n",argv[0], d->__nextchar);
                }
                else
                {
                    fprintf(stderr, "%s: unrecognized option '%c%s'\n",argv[0], argv[d->optind][0], d->__nextchar);
                }
            }
            d->__nextchar = (char *)"";
            d->optind++;
            d->optopt = 0;
            return '?';
        }
    }
    {
        char c = *d->__nextchar++;
        char *temp = (char*)strchr(optstring, c);
        if (*d->__nextchar == '\0')
            ++d->optind;
        if (temp == NULL || c == ':' || c == ';')
        {
            if (print_errors)
            {
                fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], c);
            }
            d->optopt = c;
            return '?';
        }
        if (temp[0] == 'W' && temp[1] == ';')
        {
            char *nameend;
            const struct option_a *p;
            const struct option_a *pfound = NULL;
            int exact = 0;
            int ambig = 0;
            int indfound = 0;
            int option_index;
            if (longopts == NULL)
                goto no_longs;
            if (*d->__nextchar != '\0')
            {
                d->optarg = d->__nextchar;
                d->optind++;
            }
            else if (d->optind == argc)
            {
                if (print_errors)
                {
                    fprintf(stderr,"%s: option requires an argument -- '%c'\n",argv[0], c);
                }
                d->optopt = c;
                if (optstring[0] == ':')
                    c = ':';
                else
                    c = '?';
                return c;
            }
            else
                d->optarg = argv[d->optind++];
            for (d->__nextchar = nameend = d->optarg; *nameend && *nameend != '='; nameend++);
            for (p = longopts, option_index = 0; p->name; p++, option_index++)
                if (!strncmp(p->name, d->__nextchar, nameend - d->__nextchar))
                {
                    if ((unsigned int) (nameend - d->__nextchar) == strlen(p->name))
                    {
                        pfound = p;
                        indfound = option_index;
                        exact = 1;
                        break;
                    }
                    else if (pfound == NULL)
                    {
                        pfound = p;
                        indfound = option_index;
                    }
                    else if (long_only || pfound->has_arg != p->has_arg || pfound->flag != p->flag || pfound->val != p->val)
                        ambig = 1;
                }
            if (ambig && !exact)
            {
                if (print_errors)
                {
                    fprintf(stderr, "%s: option '-W %s' is ambiguous\n",argv[0], d->optarg);
                }
                d->__nextchar += strlen(d->__nextchar);
                d->optind++;
                return '?';
            }
            if (pfound != NULL)
            {
                option_index = indfound;
                if (*nameend)
                {
                    if (pfound->has_arg)
                        d->optarg = nameend + 1;
                    else
                    {
                        if (print_errors)
                        {
                            fprintf(stderr, "%s: option '-W %s' doesn't allow an argument\n",argv[0], pfound->name);
                        }
                        d->__nextchar += strlen(d->__nextchar);
                        return '?';
                    }
                }
                else if (pfound->has_arg == 1)
                {
                    if (d->optind < argc)
                        d->optarg = argv[d->optind++];
                    else
                    {
                        if (print_errors)
                        {
                            fprintf(stderr, "%s: option '-W %s' requires an argument\n",argv[0], pfound->name);
                        }
                        d->__nextchar += strlen(d->__nextchar);
                        return optstring[0] == ':' ? ':' : '?';
                    }
                }
                else
                    d->optarg = NULL;
                d->__nextchar += strlen(d->__nextchar);
                if (longind != NULL)
                    *longind = option_index;
                if (pfound->flag)
                {
                    *(pfound->flag) = pfound->val;
                    return 0;
                }
                return pfound->val;
            }
            no_longs:
            d->__nextchar = NULL;
            return 'W';
        }
        if (temp[1] == ':')
        {
            if (temp[2] == ':')
            {
                if (*d->__nextchar != '\0')
                {
                    d->optarg = d->__nextchar;
                    d->optind++;
                }
                else
                    d->optarg = NULL;
                d->__nextchar = NULL;
            }
            else
            {
                if (*d->__nextchar != '\0')
                {
                    d->optarg = d->__nextchar;
                    d->optind++;
                }
                else if (d->optind == argc)
                {
                    if (print_errors)
                    {
                        fprintf(stderr,"%s: option requires an argument -- '%c'\n",argv[0], c);
                    }
                    d->optopt = c;
                    if (optstring[0] == ':')
                        c = ':';
                    else
                        c = '?';
                }
                else
                    d->optarg = argv[d->optind++];
                d->__nextchar = NULL;
            }
        }
        return c;
    }
}
int _getopt_internal_a (int argc, char *const *argv, const char *optstring, const struct option_a *longopts, int *longind, int long_only, int posixly_correct)
{
    int result;
    getopt_data_a.optind = optind;
    getopt_data_a.opterr = opterr;
    result = _getopt_internal_r_a (argc, argv, optstring, longopts,longind, long_only, &getopt_data_a,posixly_correct);
    optind = getopt_data_a.optind;
    optarg_a = getopt_data_a.optarg;
    optopt = getopt_data_a.optopt;
    return result;
}
int getopt_a (int argc, char *const *argv, const char *optstring) throw()
{
    return _getopt_internal_a (argc, argv, optstring, (const struct option_a *) 0, (int *) 0, 0, 0);
}
int getopt_long_a (int argc, char *const *argv, const char *options, const struct option_a *long_options, int *opt_index) throw()
{
return _getopt_internal_a (argc, argv, options, long_options, opt_index, 0, 0);
}
int getopt_long_only_a (int argc, char *const *argv, const char *options, const struct option_a *long_options, int *opt_index) throw()
{
return _getopt_internal_a (argc, argv, options, long_options, opt_index, 1, 0);
}
int _getopt_long_r_a (int argc, char *const *argv, const char *options, const struct option_a *long_options, int *opt_index, struct _getopt_data_a *d)
{
    return _getopt_internal_r_a (argc, argv, options, long_options, opt_index,0, d, 0);
}
int _getopt_long_only_r_a (int argc, char *const *argv, const char *options, const struct option_a *long_options, int *opt_index, struct _getopt_data_a *d)
{
    return _getopt_internal_r_a (argc, argv, options, long_options, opt_index, 1, d, 0);
}

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
{

}

CommandLineParser::~CommandLineParser()
{
    _nonOptions.clear();
    _options.clear();
}

void CommandLineParser::AddOptionNoArgument(OSAL::String longName, int &variable, int value,
                                            const OSAL::String & description)
{

}

void CommandLineParser::AddOptionNoArgument(OSAL::String longName, bool &variable, bool value,
                                            const OSAL::String & description)
{

}

void CommandLineParser::AddOptionNoArgument(OSAL::Char shortName, OSAL::String longName,
                                            const OSAL::String & description)
{

}

void CommandLineParser::AddOptionWithArgument(OSAL::Char shortName, OSAL::String longName, OSAL::String &variable,
                                              const OSAL::String & description)
{

}

bool CommandLineParser::HaveOption(OSAL::Char shortName) const
{
    return false;
}

bool CommandLineParser::HaveOption(OSAL::String longName) const
{
    return false;
}

bool CommandLineParser::Parse(int argc, const OSAL::Char *argv[])
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

bool CommandLineParser::Parse(int argc, OSAL::Char *argv[])
{
    return Parse(argc, const_cast<const OSAL::Char **>(argv));
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

bool CommandLineParser::InternalParse(int argc, const OSAL::Char *argv[])
{
    return true;
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
