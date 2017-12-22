#include "../export/staruml/Model.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>

using namespace std;
using namespace StarUML;

Model::Model(Object::Ptr parent, const OSAL::GUID & id, const std::string & name)
    : Container(parent, "UMLModel", id, name)
    , _visibility(ObjectVisibility::Public)
{
}

Model::~Model()
{

}

void Model::Serialize(std::ostream & stream)
{
    auto model = CreateObject();
    model->Serialize(stream, 0, 0);
}

JSON::ValuePtr Model::CreateObject() const
{
    auto result = dynamic_pointer_cast<JSON::Object>(Container::CreateObject());
    result->AddPair(JSON::KVPair("visibility", make_shared<JSON::String>(ToString(Visibility()))));
    return result;
}
