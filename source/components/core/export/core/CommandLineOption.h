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
    using Ptr = std::shared_ptr<CommandLineOption>;

    CommandLineOption() = delete;
    CommandLineOption(const CommandLineOption & other) = delete;
    CommandLineOption & operator = (const CommandLineOption & other) = delete;

    CommandLineOption(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                      CommandLineArgumentType argType = CommandLineArgumentType::NoArgument);
    CommandLineOption(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                      OSAL::String & textVariable,
                      CommandLineArgumentType argType = CommandLineArgumentType::RequiredArgument);
    virtual ~CommandLineOption() {}

public:
    const OSAL::String & LongName() const { return _longName; }
    OSAL::Char ShortName() const { return _shortName; }
    const OSAL::String & Description() const { return _description; }
    CommandLineArgumentType ArgType() const { return _argType; }
    virtual bool IsSwitchNoVariable() const { return false; }
    virtual bool IsSwitchWithVariable() const  { return false; }
    virtual bool IsEqual(Ptr other) const
    {
        return ArgType() == other->ArgType();
    }
    virtual bool IsNotEqual(Ptr other) const { return !IsEqual(other); }
    OSAL::String TextValue() const
    {
        if (_textValue)
            return *_textValue;
        return {};
    }
    void TextValue(const OSAL::String & value)
    {
        if (_textValue)
            *_textValue = value;
    }
    bool FoundOption() const { return _optionFound; }
    virtual void SetOptionFound()
    {
        _optionFound = true;
    }
    OSAL::String Argument() { return _argument; }
    virtual void Argument(const OSAL::String & value)
    {
        _argument = value;
        if (nullptr != _textValue)
            *_textValue = value;
    }

protected:
    void LongName(const OSAL::String & name) { _longName = name; }
    void ShortName(OSAL::Char name) { _shortName = name; }
    void Description(const OSAL::String & description) { this->_description = description; }

private:
    OSAL::String _longName;
    OSAL::Char _shortName;
    OSAL::String _description;
    CommandLineArgumentType _argType;
    OSAL::String * _textValue;
    bool _optionFound;
    OSAL::String _argument;
};

class CommandLineSwitchNoVariable : public CommandLineOption
{
public:
    CommandLineSwitchNoVariable() = delete;
    CommandLineSwitchNoVariable(const CommandLineSwitchNoVariable &) = delete;
    CommandLineSwitchNoVariable & operator = (const CommandLineSwitchNoVariable &) = delete;

    CommandLineSwitchNoVariable(const OSAL::String & longName, const OSAL::Char shortName, const OSAL::String & description)
        : CommandLineOption(longName, shortName, description, CommandLineArgumentType::NoArgument)
    {
    }
    virtual bool IsSwitchNoVariable() const override { return true; }

private:
};

template <class T, class Deserializer = StringDeserializer<T>>
class CommandLineSwitchWithVariable : public CommandLineOption
{
public:
    CommandLineSwitchWithVariable() = delete;
    CommandLineSwitchWithVariable(const CommandLineSwitchWithVariable &) = delete;
    CommandLineSwitchWithVariable & operator = (const CommandLineSwitchWithVariable &) = delete;

    CommandLineSwitchWithVariable(const OSAL::String & longName, const OSAL::String & description,
                                            T & variable, T value)
        : CommandLineOption(longName, '\0', description, CommandLineArgumentType::NoArgument)
        , _variable(variable)
        , _value(value)
    {
    }

    virtual void SetOptionFound() override
    {
        _variable = _value;
        CommandLineOption::SetOptionFound();
    }
    virtual bool IsSwitchWithVariable() const override { return true; }

    T & Variable() { return _variable; }
    T Value() const { return _value; }
    virtual bool IsEqual(Ptr other) const override
    {
        if (ArgType() != other->ArgType())
            return false;
        auto otherSwitchWithVariable = std::dynamic_pointer_cast<CommandLineSwitchWithVariable>(other);
        if (nullptr == otherSwitchWithVariable)
            return false;
        return (&_variable == &(otherSwitchWithVariable->_variable)) &&
               (Value() == otherSwitchWithVariable->Value());
    }

private:
    T & _variable;
    T _value;
};

} // namespace Core