#pragma once

#include <memory>
#include <vector>
#include "core/CommandLineOption.h"

namespace Core
{

typedef std::vector<CommandLineOption::Ptr> CommandLineOptionsList;

class CommandLineOptionGroup
{
public:
    using Ptr = std::shared_ptr<CommandLineOptionGroup>;
    using List = std::vector<Ptr>;

    CommandLineOptionGroup() = delete;
    CommandLineOptionGroup(const std::string & name, const std::string description = "");
    CommandLineOptionGroup(const CommandLineOptionGroup &) = delete;
    virtual ~CommandLineOptionGroup();

    CommandLineOptionGroup & operator = (const CommandLineOptionGroup &) = delete;

    const std::string & Name() const
    {
        return name;
    }
    void Name(const std::string & name)
    {
        this->name = name;
    }
    const std::string & Description() const
    {
        return description;
    }
    void Description(const std::string & description)
    {
        this->description = description;
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

    const CommandLineOptionsList & Options() const
    {
        return options;
    }

protected:
    std::string name;
    std::string description;
    CommandLineOptionsList options;
};

} // namespace Core
