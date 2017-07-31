#pragma once

#include <string>
#include <vector>
#include "core/CommandLineOption.h"
#include "core/CommandLineOptionGroup.h"
#include "osal/Console.h"

namespace Core {

    class CommandLineParser
    {
    private:
        using CommandLineOptionsList = std::vector<CommandLineOption::Ptr>;
        using CommandLineNonOptionsList = std::vector<OSAL::String>;
        static const OSAL::String DefaultMainOptionsGroupName;
        static const OSAL::String HelpOptionsGroupName;

    public:
        CommandLineParser(OSAL::Console & console,
                          const OSAL::String & name = DefaultMainOptionsGroupName,
                          const OSAL::String & description = _(""));
        CommandLineParser(const CommandLineParser &) = delete;
        CommandLineParser & operator = (const CommandLineParser &) = delete;

        virtual ~CommandLineParser();

        void AddSwitch(const OSAL::String &longName, OSAL::Char shortName,
                       const OSAL::String &description)
        {
            AddOption(std::make_shared<CommandLineSwitchNoVariable>(longName, shortName, description));
        }

        template <class T>
        void AddSwitchWithVariable(const OSAL::String &longName, T &variable, T value,
                                   const OSAL::String &description)
        {
            AddOption(std::make_shared<CommandLineSwitchWithVariable<T>>(longName, description, variable, value));
        }


        void AddOptionOptionalArgument(const OSAL::String & longName, OSAL::Char shortName,
                                       const OSAL::String & description, OSAL::String & textVariable)
        {
            AddOption(std::make_shared<CommandLineOption>(longName, shortName, description, textVariable,
                                                          CommandLineArgumentType::OptionalArgument));
        }

        void AddOptionOptionalArgument(const OSAL::String & longName,
                                       const OSAL::String & description, OSAL::String & textVariable)
        {
            AddOption(std::make_shared<CommandLineOption>(longName, _('\0'), description, textVariable,
                                                          CommandLineArgumentType::OptionalArgument));
        }

        void AddOptionRequiredArgument(OSAL::String longName, OSAL::Char shortName,
                                       const OSAL::String & description, OSAL::String & textVariable)
        {
            AddOption(std::make_shared<CommandLineOption>(longName, shortName, description, textVariable,
                                                          CommandLineArgumentType::RequiredArgument));
        }

        void AddOptionRequiredArgument(OSAL::String longName,
                                       const OSAL::String & description, OSAL::String & textVariable)
        {
            AddOption(std::make_shared<CommandLineOption>(longName, _('\0'), description, textVariable,
                                                          CommandLineArgumentType::RequiredArgument));
        }

        size_t NumNonOptions() const
        {
            return _nonOptions.size();
        }

        OSAL::String GetNonOption(size_t index) const
        {
            if (index < _nonOptions.size())
                return _nonOptions[index];
            return {};
        }

        bool HaveOption(OSAL::Char shortName) const;
        bool HaveOption(OSAL::String longName) const;
        bool FoundOption(OSAL::Char shortName) const;
        bool FoundOption(OSAL::String longName) const;
        OSAL::String GetOption(OSAL::Char shortName) const;
        OSAL::String GetOption(OSAL::String longName) const;

        void AddOption(const CommandLineOption::Ptr option);

        void AddGroup(const CommandLineOptionGroup::Ptr group);
        const CommandLineOptionGroup::Ptr MainGroup() const { return _mainGroup; }
        void MainGroup(const CommandLineOptionGroup::Ptr group);
        const CommandLineOptionGroup::List & Groups() const { return _groups; }
        bool IsInGroups(const CommandLineOptionGroup::Ptr group) const;

        bool Parse(int argc, const OSAL::Char * argv[]);

        bool AutoHandleHelp() const
        {
            return _autoHelp;
        }
        void AutoHandleHelp(bool autoHelp)
        {
            this->_autoHelp = autoHelp;
        }
        bool ShouldShowHelp() const
        {
            return _showHelp;
        }
        OSAL::String GetHelp(const OSAL::String & applicationName, bool mainHelp = true) const;
        OSAL::String GetHelp(const OSAL::String & applicationName, bool mainHelp,
                             const CommandLineOptionGroup::Ptr group) const;

        virtual void OnParseOption(const CommandLineOption::Ptr UNUSED(option))
        {
//            _console << "Option " << option->LongName() << " argument " << option->Argument() << std::endl;
        }
        virtual void OnParseNonOption(const OSAL::String UNUSED(parameter))
        {
//            _console << "Non-option " << parameter << std::endl;
        }

        enum class Ordering { RequireOrder, Permute, ReturnInOrder };
        class GetOptData
        {
        public:
            GetOptData()
                : optionIndex()
                , printErrors(true)
                , optopt()
                , optionArgument()
                , nextChar()
                , ordering()
                , firstNonOption()
                , lastNonOption()
            {
            }
            void Initialize()
            {
                optionIndex = firstNonOption = lastNonOption = 1;
                nextChar = nullptr;
                ordering = CommandLineParser::Ordering::Permute;
            }
            size_t optionIndex;
            bool printErrors;
            int optopt;
            const OSAL::Char * optionArgument;
            bool initialized;
            const OSAL::Char * nextChar;
            Ordering ordering;
            size_t firstNonOption;
            size_t lastNonOption;
        };

    protected:
        CommandLineOptionGroup::List _groups;
        CommandLineOptionGroup::Ptr _mainGroup;

    private:
        OSAL::Console & _console;
        bool _autoHelp;
        bool _showHelp;
        OSAL::String _name;
        OSAL::String _description;
        CommandLineNonOptionsList _nonOptions;
        CommandLineOption::List _options;
        GetOptData _getOptData;

        bool InternalParse(int argc, const OSAL::Char * argv[]);
        void ExchangeOption(const OSAL::Char ** argv);
        bool AtNonOption(size_t argCount, const OSAL::Char ** argv) const;
        bool AtLongOption(size_t argCount, const OSAL::Char ** argv) const;
        int HandleLongOption(size_t argCount, const OSAL::Char ** argv, const OSAL::Char * optionString,
                             size_t & optionIndex, bool printErrors);
        void SetArgument();
        int HandleShortOption(size_t argCount, const OSAL::Char ** argv, const OSAL::Char * optionString,
                              bool printErrors);
        int GetOpt(size_t argCount, const OSAL::Char ** argv, const OSAL::String & optionString,
                   size_t & optionIndex);
        void ShowHelp(const OSAL::String & applicationName);
        size_t MatchShortOption(OSAL::Char name) const;
        size_t MatchLongOption(const OSAL::String & name) const;
        size_t MatchLongOption(const OSAL::Char * name, size_t nameLength) const;
        void SelectOption(CommandLineOption::Ptr option, const OSAL::Char * value);
        void AddNonOption(const OSAL::Char * value);

        void FillOptionsList();
        void SetupStandardGroups();
    };

} // namespace Core