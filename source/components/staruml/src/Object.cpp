#include "../export/staruml/Object.h"

#include <core/Util.h>
#include <json/String.h>

using namespace std;
using namespace StarUML;

std::ostream & StarUML::operator << (std::ostream & stream, ObjectVisibility value)
{
    switch(value)
    {
        case ObjectVisibility::Public:
            stream << "public"; break;
        case ObjectVisibility::Protected:
            stream << "protected"; break;
        case ObjectVisibility::Private:
            stream << "private"; break;
        case ObjectVisibility::Package:
            stream << "package"; break;
    }
    return stream;
}

Object::Object(Ptr parent, const std::string & type)
    : _type(type)
    , _id()
    , _name()
    , _parent(parent)
{
}

Object::Object(Ptr parent, const std::string & type, const OSAL::GUID & id)
    : _type(type)
    , _id(id)
    , _name()
    , _parent(parent)
{
}

Object::Object(Ptr parent, const std::string & type, const OSAL::GUID & id, const std::string & name)
    : _type(type)
    , _id(id)
    , _name(name)
    , _parent(parent)
{
}

Object::~Object()
{

}

void Object::Serialize(std::ostream & stream)
{
    JSON::ValuePtr object = CreateObject();
    object->Serialize(stream, 0, 0);
}

JSON::ValuePtr Object::CreateObject() const
{
    auto result = make_shared<JSON::Object>();
    result->AddPair(JSON::KVPair("_type", make_shared<JSON::String>(Type())));
    result->AddPair(JSON::KVPair("_id", make_shared<JSON::String>(Core::Util::Base64Encode(Id().GetBytes()))));
    if (auto parent = _parent.lock())
    {
        auto object = make_shared<JSON::Object>();
        result->AddPair(JSON::KVPair("_parent", object));
        object->AddPair(JSON::KVPair("$ref", make_shared<JSON::String>(Core::Util::Base64Encode(parent->Id().GetBytes()))));
    }
    result->AddPair(JSON::KVPair("name", make_shared<JSON::String>(Name())));
    return result;
}

std::string Object::GetAttribute(JSON::ValuePtr object, const std::string & name) const
{
    auto objectPtr = dynamic_pointer_cast<JSON::Object>(object);
    if (objectPtr == nullptr)
        return {};

    auto valuePtr = objectPtr->Find(name);
    if (valuePtr == nullptr)
        return {};
    auto stringPtr = dynamic_pointer_cast<JSON::String>(valuePtr);
    if (stringPtr == nullptr)
        return {};
    return stringPtr->GetValue();
}

void Object::SetAttribute(JSON::ValuePtr object, const std::string & name, const std::string & value) const
{
    auto objectPtr = dynamic_pointer_cast<JSON::Object>(object);
    if (objectPtr == nullptr)
        return;

    auto valuePtr = objectPtr->Find(name);
    if (valuePtr == nullptr)
    {
        valuePtr = make_shared<JSON::String>(value);
        objectPtr->AddPair(JSON::KVPair(name, valuePtr));
        return;
    }
    auto stringPtr = dynamic_pointer_cast<JSON::String>(valuePtr);
    if (stringPtr == nullptr)
        return;
    stringPtr->SetValue(value);
}
