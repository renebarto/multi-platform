#include "json/Array.h"
#include "Parse.h"
#include "json/Boolean.h"
#include "json/Null.h"
#include "json/Number.h"
#include "json/String.h"
#include "core/Trace.h"

namespace JSON
{

static constexpr size_t IndentSize = 4;

Array::Array()
    : _elements()
{

}

Array::Array(const std::initializer_list<ValuePtr> & elements)
: _elements()
{
    for (auto & element : elements)
    {
        AddValue(element);
    }
}

void Array::AddValue(ValuePtr value)
{
    _elements.push_back(value);
}

bool Array::Deserialize(std::basic_istream<char> & stream)
{
    std::string key;
    Token token = GetToken(stream);
    if (token.type != TokenType::SquareBracketOpen)
        return false;

    bool success = true;
    bool empty = true;
    while (success)
    {
        ValuePtr value;
        value = Parse(stream);
        if (value != nullptr)
        {
            AddValue(value);
            empty = false;
        }
        else
            return empty;
        token = GetToken(stream);
        if (token.type == TokenType::SquareBracketClose)
            return true;
        if (token.type != TokenType::Comma)
        {
            while (token.type != TokenType::InvalidToken)
            {
                token = GetToken(stream);
                if (token.type == TokenType::SquareBracketClose)
                    break;
            }
            return false;
        }
    }

    token = GetToken(stream);
    return (token.type == TokenType::CurlyBraceClose);
}

void Array::Serialize(std::basic_ostream<char> & stream, int indentDepth, bool indentInitial) const
{
    if (indentInitial)
    {
        for (int i = 0; i < indentDepth; i++)
        {
            stream << std::string(IndentSize, ' ');
        }
    }
    stream << '[' << std::endl;
    ConstIterator iterator = GetIterator();
    while (!iterator.AtEnd())
    {
        (*iterator)->Serialize(stream, indentDepth + 1);
        iterator++;
        if (!iterator.AtEnd())
            stream << ',';
        stream << std::endl;
    }
    for (int i = 0; i < indentDepth; i++)
    {
        stream << std::string(IndentSize, ' ');
    }
    stream << ']';
}

void Array::Set(ValuePtr other)
{
    if (other->Type() == ValueType::Array)
    {
        _elements.clear();
        auto otherObject = std::dynamic_pointer_cast<const Array>(other);
        ConstIterator iterator = otherObject->GetIterator();

        while (!iterator.AtEnd())
        {
            AddValue((*iterator)->Clone());
            iterator++;
        }

    }
}

std::shared_ptr<Value> Array::Clone() const
{
    auto result = std::make_shared<Array>();
    ConstIterator iterator = GetIterator();
    while (!iterator.AtEnd())
    {
        result->AddValue((*iterator)->Clone());
        iterator++;
    }
    return result;
}

std::shared_ptr<Value> Array::operator[] (size_t index)
{
    if (index >= _elements.size())
    {
        Core::TraceError("Array lookup operator []: index out of bounds {}, should be between {} and {}", index, 0,
                         _elements.size());
        return nullptr;
    }
    return _elements[index];
}


bool Array::EqualTo(std::shared_ptr<Value> other) const
{
    std::shared_ptr<Array> otherAsThis = std::dynamic_pointer_cast<Array>(other);
    if (!otherAsThis)
    {
        Core::TraceError("other value is not convertible to Number");
        return false;
    }
    auto iteratorThis = GetIterator();
    auto iteratorOther = otherAsThis->GetIterator();
    while (!iteratorThis.AtEnd() && !iteratorOther.AtEnd())
    {
        if (!(*iteratorThis)->EqualTo((*iteratorOther)))
            return false;

        iteratorThis++;
        iteratorOther++;
    }
    return (iteratorThis.AtEnd() && iteratorOther.AtEnd());
}

} // namespace JSON
