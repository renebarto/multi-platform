#include "staruml/Package.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>

using namespace std;
using namespace StarUML;

Package::Package(Ptr parent)
    : Container(parent, "UMLPackage")
      , _visibility(ObjectVisibility::Public)
{
}

Package::Package(Ptr parent, const OSAL::GUID & id)
    : Container(parent, "UMLPackage", id)
    , _visibility(ObjectVisibility::Public)
{
}

Package::Package(Ptr parent, const OSAL::GUID & id, const std::string & name)
    : Container(parent, "UMLPackage", id, name)
      , _visibility(ObjectVisibility::Public)
{
}

Package::~Package()
{

}

void Package::Serialize(std::ostream & stream)
{
    auto ns = CreateObject();
    ns->Serialize(stream, 0, 0);
}

JSON::ValuePtr Package::CreateObject() const
{
    auto result = dynamic_pointer_cast<JSON::Object>(Container::CreateObject());
    result->AddPair(JSON::KVPair("visibility", make_shared<JSON::String>(ToString(Visibility()))));
    return result;
}
