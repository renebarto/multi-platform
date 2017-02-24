#include "core/CommandLineOption.h"

#include "core/serialization/Serialization.h"

using namespace Core;

template<>
std::vector<EnumConversion<CommandLineArgumentType>> EnumSerializationInfo<CommandLineArgumentType>::Info =
{
    { CommandLineArgumentType::NoArgument, "None" },
    { CommandLineArgumentType::OptionalArgument, "Optional" },
    { CommandLineArgumentType::RequiredArgument, "Required" },
};

CommandLineOption::CommandLineOption(const std::string & longName, char shortName,
                                     const std::string & description,
                                     CommandLineArgumentType argType)
    : _longName(longName)
    , _shortName(shortName)
    , _description(description)
    , _argType(argType)
    , _variable(nullptr)
    , _value()
    , _textValue(nullptr)
    , _optionFound()
//    , _optionFoundFlag(nullptr)
    , _argument()
{
}

CommandLineOption::CommandLineOption(const OSAL::String & longName, const OSAL::String & description,
                                     int & variable, int value,
                                     CommandLineArgumentType argType)
    : _longName(longName)
    , _shortName('\0')
    , _description(description)
    , _argType(argType)
    , _variable(&variable)
    , _value(value)
    , _textValue(nullptr)
    , _optionFound()
//    , _optionFoundFlag(nullptr)
    , _argument()
{

}

CommandLineOption::CommandLineOption(const OSAL::String & longName, OSAL::Char shortName, const OSAL::String & description,
                                     OSAL::String & textVariable,
                                     CommandLineArgumentType argType)
    : _longName(longName)
    , _shortName(shortName)
    , _description(description)
    , _argType(argType)
    , _variable(nullptr)
    , _value()
    , _textValue(&textVariable)
    , _optionFound()
//    , _optionFoundFlag(nullptr)
    , _argument()
{

}

//CommandLineOption::CommandLineOption(const std::string & longName, char shortName,
//                                     const std::string & description,
//                                     bool * optionFoundFlag,
//                                     CommandLineArgumentType argType)
//    : _longName(longName)
//    , _shortName(shortName)
//    , _description(description)
//    , _argType(argType)
////    , _optionFound(false)
////    , _optionFoundFlag(optionFoundFlag)
//{
//    if (optionFoundFlag != nullptr)
//        *optionFoundFlag = false;
//}

CommandLineOption::CommandLineOption(const CommandLineOption & other)
{
    operator = (other);
}

CommandLineOption::~CommandLineOption()
{
}

CommandLineOption & CommandLineOption::operator = (const CommandLineOption & other)
{
    if (this != &other)
    {
        this->_longName = other._longName;
        this->_shortName = other._shortName;
        this->_description = other._description;
        this->_argType = other._argType;
        this->_optionFound = other._optionFound;
//        this->_optionFoundFlag = other._optionFoundFlag;
        this->_argument = other._argument;
    }

    return *this;
}
