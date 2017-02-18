#pragma once

#include <string>
#include <vector>
#include "core/CommandLineOption.h"
#include "osal/console-darwin.h"

namespace Core {

    class CommandLineParser
    {
    private:
        static const OSAL::String DefaultMainOptionsGroupName;

    public:
        CommandLineParser(OSAL::Console & console,
                          const OSAL::String & name = DefaultMainOptionsGroupName,
                          const OSAL::String & description = _(""));
        CommandLineParser(const CommandLineParser &) = delete;
        CommandLineParser & operator = (const CommandLineParser &) = delete;

        ~CommandLineParser();

        void AddOptionNoArgument(OSAL::String longName, int &variable, int value,
                                 const OSAL::String & description);

        void AddOptionNoArgument(OSAL::String longName, bool &variable, bool value,
                                 const OSAL::String & description);

        void AddOptionNoArgument(OSAL::Char shortName, OSAL::String longName,
                                 const OSAL::String & description);

        template <class T>
        void AddOptionRequiredArgument(const OSAL::String & longName, OSAL::Char shortName,
                                       const OSAL::String & description, T * argument)
        {
            AddOption(CommandLineOptionPtr(
                new CommandLineOptionWithArgument<T>(longName, shortName, description, argument)));
        }


        void AddOptionWithArgument(OSAL::Char shortName, OSAL::String longName, OSAL::String &variable,
                                   const OSAL::String & description);

        size_t NumNonOptions() const
        {
            return _nonOptions.size();
        }

        bool HaveOption(OSAL::Char shortName) const;
        bool HaveOption(OSAL::String longName) const;

        bool Parse(int argc, const OSAL::Char *argv[]);
        bool Parse(int argc, OSAL::Char *argv[]);

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

    private:
        OSAL::Console & _console;
        bool _autoHelp;
        bool _showHelp;
        OSAL::String _name;
        OSAL::String _description;
        std::vector<OSAL::String> _nonOptions;
        std::vector<CommandLineOptionPtr> _options;

        bool InternalParse(int argc, const OSAL::Char *argv[]);
        void ShowHelp(const OSAL::String & applicationName);
    };

} // namespace Core