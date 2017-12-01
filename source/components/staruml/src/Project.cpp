#include "../export/staruml/Project.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>
#include "../export/staruml/ClassDiagram.h"
#include "../export/staruml/Model.h"

using namespace std;
using namespace StarUML;

Project::Project(const OSAL::ByteArray & id, const std::string & name)
    : Container(nullptr, "Project", id, name)
{
}

Project::~Project()
{

}

void Project::Serialize(std::ostream & stream)
{
    auto document = CreateObject();
    document->Serialize(stream, 0, 0);
}

JSON::ValuePtr Project::CreateObject() const
{
    auto result = dynamic_pointer_cast<JSON::Object>(Container::CreateObject());
    return result;
}

void Project::SetupDefault()
{
    OSAL::ByteArray id = Core::Util::Base64Decode("AAAAAAFF+qBWK6M3Z8Y=");
    auto model = make_shared<Model>(shared_from_this(), id, "Model");
    AddElement(model);
    id = Core::Util::Base64Decode("AAAAAAFF+qBtyKM79qY=");
    auto classDiagram = make_shared<ClassDiagram>(model, id, "Main");
    classDiagram->Visible(true);
    classDiagram->DefaultDiagram(true);
    model->AddElement(classDiagram);
}
