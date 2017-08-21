#pragma once

#include <memory>
#include <vector>
#include "core/CommandLineOption.h"

namespace Core
{

typedef std::vector<CommandLineOption::Ptr> CommandLineOptionsList;

class CORE_EXPORT CommandLineOptionGroup
{
public:
    using Ptr = std::shared_ptr<CommandLineOptionGroup>;
    using List = std::vector<Ptr>;

    CommandLineOptionGroup() = delete;
    CommandLineOptionGroup(const OSAL::String & name, const OSAL::String description = _(""));
    CommandLineOptionGroup(const CommandLineOptionGroup &) = delete;
    virtual ~CommandLineOptionGroup();

    CommandLineOptionGroup & operator = (const CommandLineOptionGroup &) = delete;

    const OSAL::String & Name() const
    {
        return _name;
    }
    void Name(const OSAL::String & name)
    {
        this->_name = name;
    }
    const OSAL::String & Description() const
    {
        return _description;
    }
    void Description(const OSAL::String & description)
    {
        this->_description = description;
    }

    void AddOption(const CommandLineOption::Ptr option);
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

    template<class T>
    void AddOptionOptionalArgumentWithVariable(const OSAL::String & longName, OSAL::Char shortName,
                                               const OSAL::String & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, shortName, description, variable,
                                                                  CommandLineArgumentType::OptionalArgument));
    }

    template<class T>
    void AddOptionOptionalArgumentWithVariable(const OSAL::String & longName,
                                               const OSAL::String & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, _('\0'), description, variable,
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

    template<class T>
    void AddOptionRequiredArgumentWithVariable(const OSAL::String & longName, OSAL::Char shortName,
                                               const OSAL::String & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, shortName, description, variable,
                                                                  CommandLineArgumentType::RequiredArgument));
    }

    template<class T>
    void AddOptionRequiredArgumentWithVariable(const OSAL::String & longName,
                                               const OSAL::String & description, T & variable)
    {
        AddOption(std::make_shared<CommandLineOptionWithVariable<T>>(longName, _('\0'), description, variable,
                                                                  CommandLineArgumentType::RequiredArgument));
    }

    const CommandLineOptionsList & Options() const
    {
        return _options;
    }

protected:
	OSAL::String _name;
	OSAL::String _description;
    CommandLineOptionsList _options;
};

} // namespace Core
