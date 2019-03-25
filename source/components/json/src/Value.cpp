#include "json/Value.h"
#include "json/Boolean.h"
#include "json/Number.h"
#include "json/Null.h"
#include "json/String.h"
#include "json/Array.h"
#include "json/Object.h"
#include "core/Trace.h"

namespace JSON
{

std::shared_ptr<Value> Value::operator[] (const std::string & UNUSED(key))
{
    Core::TraceError("Object lookup operator [] not defined for this type");
    return nullptr;
}

std::shared_ptr<Value> Value::operator[] (size_t UNUSED(index))
{
    Core::TraceError("Array lookup operator [] not defined for this type");
    return nullptr;
}

bool operator == (ValuePtr lhs, ValuePtr rhs)
{
    if (lhs->Type() != rhs->Type())
        return false;
    switch (lhs->Type())
    {
        case JSON::ValueType::Null:
            return std::dynamic_pointer_cast<JSON::Null>(lhs)->EqualTo(rhs);
        case JSON::ValueType::Boolean:
            return std::dynamic_pointer_cast<JSON::Boolean>(lhs)->EqualTo(rhs);
        case JSON::ValueType::Number:
            return std::dynamic_pointer_cast<JSON::Number>(lhs)->EqualTo(rhs);
        case JSON::ValueType::String:
            return std::dynamic_pointer_cast<JSON::String>(lhs)->EqualTo(rhs);
        case JSON::ValueType::Array:
            return std::dynamic_pointer_cast<JSON::Array>(lhs)->EqualTo(rhs);
        case JSON::ValueType::Object:
            return std::dynamic_pointer_cast<JSON::Object>(lhs)->EqualTo(rhs);
        default:
            Core::Trace(Core::TraceClass::Warning, "Invalid JSON type: {}", static_cast<int>(lhs->Type()));
            return false;
    }
}
bool operator != (ValuePtr lhs, ValuePtr rhs)
{
    return ! operator == (lhs, rhs);
}

} // namespace JSON
