#include "../export/staruml/Model.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>

using namespace std;
using namespace StarUML;

std::ostream & StarUML::operator << (std::ostream & stream, const ObjectVisibility & value)
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
static std::string ObjectVisibilityToString(ObjectVisibility value)
{
    ostringstream stream;
    stream << value;
    return stream.str();
}

Model::Model(Ptr parent, const OSAL::ByteArray & id, const std::string & name)
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
    result->AddPair(JSON::KVPair("visibility", make_shared<JSON::String>(ObjectVisibilityToString(Visibility()))));
    return result;
}
