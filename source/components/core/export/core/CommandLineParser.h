#pragma once

#include <string>
#include <vector>
#include "core/CommandLineOption.h"
#include "osal/console-darwin.h"

namespace Core {

    class CommandLineParser
    {
    private:
        using CommandLineOptionsList = std::vector<CommandLineOptionPtr>;
        using CommandLineNonOptionsList = std::vector<OSAL::String>;
        static const OSAL::String DefaultMainOptionsGroupName;

    public:
        CommandLineParser(OSAL::Console & console,
                          const OSAL::String & name = DefaultMainOptionsGroupName,
                          const OSAL::String & description = _(""));
        CommandLineParser(const CommandLineParser &) = delete;
        CommandLineParser & operator = (const CommandLineParser &) = delete;

        ~CommandLineParser();

        void AddOptionNoArgument(const OSAL::String & longName, int &variable, int value,
                                 const OSAL::String & description);

//        void AddOptionNoArgument(const OSAL::String longName, bool &variable, bool value,
//                                 const OSAL::String & description);

        void AddOptionNoArgument(OSAL::Char shortName, const OSAL::String & longName,
                                 const OSAL::String & description);

        void AddOptionOptionalArgument(OSAL::Char shortName, const OSAL::String & longName,
                                       const OSAL::String & description, OSAL::String & textVariable);
        void AddOptionOptionalArgument(const OSAL::String & longName,
                                       const OSAL::String & description, OSAL::String & textVariable);

        void AddOptionRequiredArgument(OSAL::Char shortName, OSAL::String longName,
                                       const OSAL::String & description, OSAL::String & textVariable);
        void AddOptionRequiredArgument(OSAL::String longName,
                                       const OSAL::String & description, OSAL::String & textVariable);

//        template <class T>
//        void AddOptionOptionalArgument(OSAL::Char shortName, const OSAL::String & longName,
//                                       const OSAL::String & description, T * argument)
//        {
//            AddOption(std::make_shared<CommandLineOptionWithArgument<T>>(shortName, longName, description, argument, false));
//        }
//
//        template <class T>
//        void AddOptionRequiredArgument(OSAL::Char shortName, const OSAL::String & longName,
//                                       const OSAL::String & description, T * argument)
//        {
//            AddOption(std::make_shared<CommandLineOptionWithArgument<T>>(shortName, longName, description, argument));
//        }


//        void AddOptionWithArgument(OSAL::Char shortName, OSAL::String longName, OSAL::String &variable,
//                                   const OSAL::String & description);

        size_t NumNonOptions() const
        {
            return _nonOptions.size();
        }
        OSAL::String GetNonOption(size_t index) const;

        bool HaveOption(OSAL::Char shortName) const;
        bool HaveOption(OSAL::String longName) const;
        bool FoundOption(OSAL::Char shortName) const;
        bool FoundOption(OSAL::String longName) const;
        OSAL::String GetOption(OSAL::Char shortName) const;
        OSAL::String GetOption(OSAL::String longName) const;

        bool Parse(int argc, const OSAL::Char * argv[]);

        bool AutoHandleHelp()
        {
            return _autoHelp;
        }
        void AutoHandleHelp(bool autoHelp)
        {
            this->_autoHelp = autoHelp;
        }
        bool ShouldShowHelp()
        {
            return _showHelp;
        }
        OSAL::String GetHelp(const OSAL::String & applicationName) const;
        void AddOption(const CommandLineOptionPtr option);

        virtual void OnParseOption(const CommandLineOptionPtr option)
        {
            _console << "Option " << option->LongName() << " argument " << option->Argument() << std::endl;
        }
        virtual void OnParseNonOption(const std::string UNUSED(parameter)) {}

        struct option_a
        {
            const OSAL::Char * name;
            CommandLineArgumentType argumentType;
            int * flag;
            int val;
        };
        enum ENUM_ORDERING { REQUIRE_ORDER, PERMUTE, RETURN_IN_ORDER };
        struct GetOptData
        {
            int optind;
            int opterr;
            int optopt;
            const OSAL::Char * optarg;
            bool initialized;
            const OSAL::Char *__nextchar;
            ENUM_ORDERING __ordering;
            int __first_nonopt;
            int __last_nonopt;
        };
    private:
        OSAL::Console & _console;
        bool _autoHelp;
        bool _showHelp;
        OSAL::String _name;
        OSAL::String _description;
        CommandLineNonOptionsList _nonOptions;
        CommandLineOptionsList _options;
        struct GetOptData _getoptData;

        void ExchangeOption(const OSAL::Char ** argv);
        const OSAL::Char * Initialize(const OSAL::Char * optstring);
        int GetOptInternal(int argc, const OSAL::Char ** argv, const OSAL::String &optionString,
                           const CommandLineParser::option_a *longopts,
                           size_t &longind);
        int GetOpt(int argc, const OSAL::Char ** argv, const OSAL::String & optionString, const option_a *long_options, size_t & optionIndex);
        bool InternalParse(int argc, const OSAL::Char * argv[]);
        void ShowHelp(const OSAL::String & applicationName);
        size_t MatchShortOption(OSAL::Char name) const;
        size_t MatchLongOption(const OSAL::String & name) const;
        size_t MatchLongOption(const OSAL::Char * name, size_t nameLength) const;
        void SelectOption(CommandLineOptionPtr option, const OSAL::Char * value);
        void AddNonOption(const OSAL::Char * value);
    };

} // namespace Core