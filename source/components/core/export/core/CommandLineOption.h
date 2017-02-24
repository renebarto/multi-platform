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
    CommandLineOption(const OSAL::String & longName, const OSAL::String & description,
                      int & variable, int value,
                      CommandLineArgumentType argType = CommandLineArgumentType::NoArgument);
//    CommandLineOption(const OSAL::String & longName, const OSAL::String & description,
//                      bool & variable, bool value,
//                      CommandLineArgumentType argType = CommandLineArgumentType::NoArgument);
    CommandLineOption(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                      OSAL::String & textVariable,
                      CommandLineArgumentType argType = CommandLineArgumentType::RequiredArgument);
    CommandLineOption(const CommandLineOption & other);
    virtual ~CommandLineOption();

    CommandLineOption & operator = (const CommandLineOption & other);

public:
    const OSAL::String & LongName() const { return _longName; }
    OSAL::Char ShortName() const { return _shortName; }
    const OSAL::String & Description() const { return _description; }
    CommandLineArgumentType ArgType() const { return _argType; }
    int * Variable() { return _variable; }
    int Value() const { return _value; }
    OSAL::String TextValue() const
    {
        if (_textValue)
            return *_textValue;
        return {};
    }

    void Value(int value) { _value = value; }
    void TextValue(const OSAL::String & value)
    {
        if (_textValue)
            *_textValue = value;
    }
    bool FoundOption() const { return _optionFound; }
    void FoundOption(bool optionFound)
    {
        _optionFound = optionFound;
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
    int * _variable;
    int _value;
    OSAL::String * _textValue;
    bool _optionFound;
    OSAL::String _argument;
};

typedef std::shared_ptr<CommandLineOption> CommandLineOptionPtr;

//class CommandLineOptionNoArgument : public CommandLineOption
//{
//public:
//    CommandLineOptionNoArgument(const OSAL::Char shortName, OSAL::String & longName, const OSAL::String & description)
//        : CommandLineOption(longName, shortName, description)
//    {
//    }
//
//private:
//};
//
//template <class T, class Deserializer = StringDeserializer<T>>
//class CommandLineOptionNoArgumentWithVariable : public CommandLineOption
//{
//public:
//    CommandLineOptionNoArgumentWithVariable(const OSAL::String & longName, T & variable, T value, const OSAL::String & description)
//        : CommandLineOption(longName, '\0', description)
//        , variable(variable)
//        , value(value)
//    {
//    }
//
//private:
//    T & variable;
//    T value;
//};
//
//template <class T, class Deserializer = StringDeserializer<T>>
//class CommandLineOptionWithArgument : public CommandLineOption
//{
//public:
//    CommandLineOptionWithArgument() = delete;
//    CommandLineOptionWithArgument(const OSAL::Char shortName, const OSAL::String & name,
//                                  const OSAL::String & description,
//                                  T * argument, bool requiredArgument = true)
//        : CommandLineOption(name, shortName, description,
//                            requiredArgument
//                            ? CommandLineArgumentType::RequiredArgument
//                            : CommandLineArgumentType::OptionalArgument)
//        , _argument(argument)
//    {
//    }
//    CommandLineOptionWithArgument(OSAL::Char shortName, const OSAL::String & name,
//                                  const OSAL::String & description,
//                                  bool * optionFound, T * argument, bool requiredArgument = true)
//        : CommandLineOption(name, shortName, description, optionFound,
//                            requiredArgument
//                            ? CommandLineArgumentType::RequiredArgument
//                            : CommandLineArgumentType::OptionalArgument)
//        , _argument(argument)
//    {
//    }
//    CommandLineOptionWithArgument(const CommandLineOptionWithArgument & other)
//        : CommandLineOption(other)
//    {
//        _argument = other._argument;
//    }
//    virtual ~CommandLineOptionWithArgument() {}
//
//    CommandLineOptionWithArgument & operator = (const CommandLineOptionWithArgument & other)
//    {
//        if (this != &other)
//        {
//            CommandLineOption::operator =(other);
//            _argument = other._argument;
//        }
//
//        return *this;
//    }
//
//    T * Argument() { return _argument; }
//    const T * Argument() const { return _argument; }
//
//    virtual void Value(const OSAL::String & value)
//    {
//        _value = value;
//        if (nullptr == _argument)
//            return;
//        Deserializer deserializer;
//        if (!deserializer.Deserialize(value, *_argument))
//        {
//            *_argument = T{};
//        }
//    }
//
//private:
//    T * _argument;
//    OSAL::String _value;
//};

} // namespace Core