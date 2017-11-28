#include <json/String.h>
#include "staruml/Object.h"

using namespace std;
using namespace StarUML;

Object::Object()
    : JSON::Object()
{
    AddPair(JSON::KVPair("_type", make_shared<JSON::String>()));
    AddPair(JSON::KVPair("_id", make_shared<JSON::String>()));
    AddPair(JSON::KVPair("_name", make_shared<JSON::String>()));
}

Object::~Object()
{

}

bool Object::GetProperty(const std::string & name, std::string & value) const
{
    JSON::ValuePtr item = Find(name);
    if (item != nullptr)
    {
        std::shared_ptr<JSON::String> stringValue = dynamic_pointer_cast<JSON::String>(item);
        if (stringValue != nullptr)
        {
            value = stringValue->GetValue();
        }
    }
    return false;
}

void Object::SetProperty(const std::string & name, const std::string & value)
{
    JSON::ValuePtr item = Find(name);
    if (item != nullptr)
    {
        std::shared_ptr<JSON::String> stringValue = dynamic_pointer_cast<JSON::String>(item);
        if (stringValue != nullptr)
        {
            stringValue->SetValue(value);
        }
    }
}

void Object::Serialize(std::ostream & stream)
{
    JSON::Object::Serialize(stream, 0, 0);
}