#include "json/Object.h"
#include "Parse.h"
#include "json/Boolean.h"
#include "json/Null.h"
#include "json/Number.h"
#include "json/String.h"

namespace JSON
{

static size_t IndentSize = 4;

KVPair::KVPair()
    : _key()
    , _value()
{
}

KVPair::KVPair(const OSAL::String & key, const ValuePtr & value)
    : _key(key)
    , _value(value)
{
}

bool KVPair::Deserialize(std::basic_istream<OSAL::Char> & stream)
{
    _key = {};
    _value = {};
    OSAL::String key;
    Token token = GetToken(stream);
    if (token.type != TokenType::QuotedString)
        return false;
    key = token.value;

    token = GetToken(stream);
    if (token.type != TokenType::Colon)
        return false;

    ValuePtr value;
    token = GetToken(stream);
    switch (token.type)
    {
        case TokenType::NullToken:
            _key = key;
            _value = std::make_shared<Null>();
            return true;
        case TokenType::FalseToken:
            _key = key;
            _value = std::make_shared<Boolean>(false);
            return true;
        case TokenType::TrueToken:
            _key = key;
            _value = std::make_shared<Boolean>(true);
            return true;
        case TokenType::Number:
            _key = key;
            _value = std::make_shared<Number>(token.value);
            return true;
        case TokenType::QuotedString:
            _key = key;
            _value = std::make_shared<String>(token.value);
            return true;
        default:
            return false;
    }
}

void KVPair::Serialize(std::basic_ostream<OSAL::Char> & stream, int indentDepth) const
{
    for (int i = 0; i < indentDepth; i++)
    {
        stream << OSAL::String(IndentSize, _(' '));
    }
    stream << _("\"") << _key << _("\" : ");
    if (_value != nullptr)
        _value->Serialize(stream, indentDepth);
    else
        stream << _("null");
}

Object::Object()
    : _elements()
{

}

Object::Object(const std::initializer_list<KVPair> & elements)
    : _elements()
{
    for (auto & element : elements)
    {
        AddPair(element);
    }
}

bool Object::AddPair(const KVPair & pair)
{
    auto result = _elements.insert(std::pair<const OSAL::String &, ValuePtr>(pair.GetKey(), pair.GetValue()));
    return result.second;
}

bool Object::Deserialize(std::basic_istream<OSAL::Char> & stream)
{
    OSAL::String key;
    Token token = GetToken(stream);
    if (token.type != TokenType::CurlyBraceOpen)
        return false;

    bool success = true;
    bool empty = true;
    while (success)
    {
        KVPair pair;
        success = pair.Deserialize(stream);
        if (success)
        {
            if (!AddPair(pair))
                return false;
            empty = false;
        }
        else
            return empty;
        token = GetToken(stream);
        if (token.type == TokenType::CurlyBraceClose)
            return true;
        if (token.type != TokenType::Comma)
        {
            while (token.type != TokenType::InvalidToken)
            {
                token = GetToken(stream);
                if (token.type == TokenType::CurlyBraceClose)
                    break;
            }
            return false;
        }
    }

    token = GetToken(stream);
    return (token.type == TokenType::CurlyBraceClose);
}

void Object::Serialize(std::basic_ostream<OSAL::Char> & stream, int indentDepth) const
{
    for (int i = 0; i < indentDepth; i++)
    {
        stream << OSAL::String(IndentSize, _(' '));
    }
    stream << _('{') << std::endl;
    ConstIterator iterator = GetIterator();
    while (!iterator.AtEnd())
    {
        (*iterator).Serialize(stream, indentDepth + 1);
        iterator++;
        if (!iterator.AtEnd())
            stream << _(',');
        stream << std::endl;
    }
    for (int i = 0; i < indentDepth; i++)
    {
        stream << OSAL::String(IndentSize, _(' '));
    }
    stream << _('}');
}

} // namespace JSON
