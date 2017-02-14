#pragma once

#include <memory>
#include <osal/strings.h>
#include "core/serialization/Serialization.h"
#include "core/serialization/DeserializationImpl.h"
#include "core/serialization/SerializationImpl.h"

namespace Core {


enum class CommandLineArgumentType
{
    NoArgument,
    RequiredArgument,
    OptionalArgument,
};
inline std::basic_ostream<OSAL::Char> & operator << (std::basic_ostream<OSAL::Char> & stream, const CommandLineArgumentType & value)
{
    stream << Serialize(value);
    return stream;
}

class CommandLineOption
{
public:
    CommandLineOption() = delete;
    CommandLineOption(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                      CommandLineArgumentType argType = CommandLineArgumentType::NoArgument);
    CommandLineOption(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                      bool * optionFoundFlag,
                      CommandLineArgumentType argType = CommandLineArgumentType::NoArgument);
    CommandLineOption(const CommandLineOption & other);
    virtual ~CommandLineOption();

    CommandLineOption & operator = (const CommandLineOption & other);

    const OSAL::String & LongName() const { return _longName; }
    OSAL::Char ShortName() const { return _shortName; }
    const OSAL::String & Description() const { return _description; }
    CommandLineArgumentType ArgType() const { return _argType; }
    bool FoundOption() const { return _optionFound; }
    bool * OptionFoundFlag() const { return _optionFoundFlag; }
    OSAL::String Value() { return _value; }

protected:
    void LongName(const OSAL::String & name) { _longName = name; }
    void ShortName(OSAL::Char name) { _shortName = name; }
    void Description(const OSAL::String & description) { this->_description = description; }
    void FoundOption(bool optionFound)
    {
        this->_optionFound = optionFound;
        if (nullptr != _optionFoundFlag)
            *_optionFoundFlag = optionFound;
    }
    void OptionFoundFlag(bool * optionFoundFlag) { this->_optionFoundFlag = optionFoundFlag; }
    virtual void Value(const OSAL::String & value) { this->_value = value; }

private:
    OSAL::String _longName;
    OSAL::Char _shortName;
    OSAL::String _description;
    CommandLineArgumentType _argType;
    bool _optionFound;
    bool * _optionFoundFlag;
    OSAL::String _value;
};

typedef std::shared_ptr<CommandLineOption> CommandLineOptionPtr;

template <class T, class Deserializer = StringDeserializer<T>>
class CommandLineOptionWithArgument : public CommandLineOption
{
public:
    CommandLineOptionWithArgument() = delete;
    CommandLineOptionWithArgument(const OSAL::String & name, OSAL::Char shortName,
                                  const OSAL::String & description,
                                  T * argument, bool requiredArgument = true)
        : CommandLineOption(name, shortName, description,
                            requiredArgument
                            ? CommandLineArgumentType::RequiredArgument
                            : CommandLineArgumentType::OptionalArgument)
        , _argument(argument)
    {
    }
    CommandLineOptionWithArgument(const OSAL::String & name, OSAL::Char shortName,
                                  const OSAL::String & description,
                                  bool * optionFound, T * argument, bool requiredArgument = true)
        : CommandLineOption(name, shortName, description, optionFound,
                            requiredArgument
                            ? CommandLineArgumentType::RequiredArgument
                            : CommandLineArgumentType::OptionalArgument)
        , _argument(argument)
    {
    }
    CommandLineOptionWithArgument(const CommandLineOptionWithArgument & other)
        : CommandLineOption(other)
    {
        _argument = other._argument;
    }
    virtual ~CommandLineOptionWithArgument() {}

    CommandLineOptionWithArgument & operator = (const CommandLineOptionWithArgument & other)
    {
        if (this != &other)
        {
            CommandLineOption::operator =(other);
            _argument = other._argument;
        }

        return *this;
    }

    T * Argument() { return _argument; }
    const T * Argument() const { return _argument; }

    virtual void Value(const OSAL::String & value)
    {
        CommandLineOption::Value(value);
        if (nullptr == _argument)
            return;
        Deserializer deserializer;
        if (!deserializer.Deserialize(value, *_argument))
        {
            *_argument = T{};
        }
    }

private:
    T * _argument;
};

} // namespace Core