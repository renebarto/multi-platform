#include "json/String.h"
#include "Parse.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>
#include <core/Trace.h>

namespace JSON
{

static constexpr size_t IndentSize = 4;

String::String()
    : _value()
{
}

String::String(const std::string & value)
    : _value(value)
{
}

bool String::Deserialize(std::basic_istream<char> & stream)
{
    Token token = GetToken(stream);
    switch (token.type)
    {
        case TokenType::QuotedString:
            _value = token.value;
            return true;
        default:
            _value = {};
            return false;
    }
}

void String::Serialize(std::basic_ostream<char> & stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << '"' << _value << '"';
}

bool String::EqualTo(std::shared_ptr<Value> other) const
{
    std::shared_ptr<String> otherAsThis = std::dynamic_pointer_cast<String>(other);
    if (!otherAsThis)
    {
        Core::TraceError("other value is not convertible to Number");
        return false;
    }
    return _value == otherAsThis->_value;
}

} // namespace JSON
