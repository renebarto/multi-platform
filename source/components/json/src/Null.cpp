#include "json/Null.h"
#include "Parse.h"
#include <core/serialization/DeserializationImpl.h>
#include <core/serialization/SerializationImpl.h>
#include <core/Trace.h>

namespace JSON
{

static constexpr size_t IndentSize = 4;

Null::Null()
{
}

bool Null::Deserialize(std::basic_istream<char> & stream)
{
    Token token = GetToken(stream);
    switch (token.type)
    {
        case TokenType::NullToken:
            return true;
        default:
            return false;
    }
}

void Null::Serialize(std::basic_ostream<char> & stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << "null";
}

bool Null::EqualTo(std::shared_ptr<Value> other) const
{
    std::shared_ptr<Null> otherAsThis = std::dynamic_pointer_cast<Null>(other);
    if (!otherAsThis)
    {
        Core::TraceError("other value is not convertible to Boolean");
        return false;
    }
    return true;
}

} // namespace JSON
