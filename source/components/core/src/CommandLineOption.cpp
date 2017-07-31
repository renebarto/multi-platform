#include "core/CommandLineOption.h"

#include <osal/OSAL.h>
#include "core/serialization/Serialization.h"

using namespace Core;

WARNING_PUSH
WARNING_DISABLE(4592)
template<>
std::vector<EnumConversion<CommandLineArgumentType>> EnumSerializationInfo<CommandLineArgumentType>::Info =
{
	{ CommandLineArgumentType::NoArgument, _("None") },
	{ CommandLineArgumentType::OptionalArgument, _("Optional") },
	{ CommandLineArgumentType::RequiredArgument, _("Required") },
};
WARNING_POP

CommandLineOption::CommandLineOption(const OSAL::String & longName, OSAL::Char shortName,
                                     const OSAL::String & description,
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

CommandLineOption::CommandLineOption(const OSAL::String & longName, OSAL::Char shortName, 
									 const OSAL::String & description,
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

