#include "json/Object.h"
#include "Parse.h"
#include "json/Boolean.h"
#include "json/Null.h"
#include "json/Number.h"
#include "json/String.h"

namespace JSON
{

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
    if (token.type != TokenType::Key)
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

void KVPair::Serialize(std::basic_ostream<OSAL::Char> & stream) const
{
    stream << _key << _(" : ");
    if (_value != nullptr)
        _value->Serialize(stream);
    else
        stream << _("null");
}

Object::Object()
    : _elements()
{

}

} // namespace JSON
