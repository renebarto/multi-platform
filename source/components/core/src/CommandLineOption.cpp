#include "core/CommandLineOption.h"

using namespace std;
using namespace Core;

WARNING_PUSH
WARNING_DISABLE(4592)
template<>
std::vector<EnumConversion<CommandLineArgumentType>> EnumSerializationInfo<CommandLineArgumentType>::Info =
{
    {CommandLineArgumentType::NoArgument,       "None"},
    {CommandLineArgumentType::OptionalArgument, "Optional"},
    {CommandLineArgumentType::RequiredArgument, "Required"},
};
WARNING_POP

CommandLineOption::CommandLineOption(const string & longName, char shortName,
                                     const string & description,
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

CommandLineOption::CommandLineOption(const string & longName, char shortName,
                                     const string & description,
                                     string & textVariable,
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

