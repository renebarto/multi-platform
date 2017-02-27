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
    , _textValue(nullptr)
    , _optionFound()
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
    , _textValue(&textVariable)
    , _optionFound()
    , _argument()
{

}

