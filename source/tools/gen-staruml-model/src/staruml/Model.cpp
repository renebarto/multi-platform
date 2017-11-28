#include "staruml/Model.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>

using namespace std;
using namespace StarUML;

Model::Model(const OSAL::ByteArray & id)
    : Container("UMLModel", id, "Model")
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
    return result;
}
