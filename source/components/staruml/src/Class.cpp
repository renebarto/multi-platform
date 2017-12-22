#include "staruml/Class.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>
#include <json/Boolean.h>

using namespace std;
using namespace StarUML;

Class::Class(Ptr parent)
    : Container(parent, "UMLClass")
      , _visibility(ObjectVisibility::Public)
{
}

Class::Class(Ptr parent, const OSAL::GUID & id)
    : Container(parent, "UMLClass", id)
    , _visibility(ObjectVisibility::Public)
{
}

Class::Class(Ptr parent, const OSAL::GUID & id, const std::string & name)
    : Container(parent, "UMLClass", id, name)
      , _visibility(ObjectVisibility::Public)
{
}

Class::~Class()
{

}

void Class::Serialize(std::ostream & stream)
{
    auto ns = CreateObject();
    ns->Serialize(stream, 0, 0);
}

JSON::ValuePtr Class::CreateObject() const
{
    auto result = dynamic_pointer_cast<JSON::Object>(Container::CreateObject());
    result->AddPair(JSON::KVPair("visibility", make_shared<JSON::String>(ToString(Visibility()))));
    result->AddPair(JSON::KVPair("operations", make_shared<JSON::Array>()));
    result->AddPair(JSON::KVPair("isAbstract", make_shared<JSON::Boolean>(false)));
    result->AddPair(JSON::KVPair("isFinalSpecialization", make_shared<JSON::Boolean>(false)));
    result->AddPair(JSON::KVPair("isLeaf", make_shared<JSON::Boolean>(false)));
    result->AddPair(JSON::KVPair("isActive", make_shared<JSON::Boolean>(false)));
    return result;
}
