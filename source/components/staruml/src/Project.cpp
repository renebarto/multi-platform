#include "../export/staruml/Project.h"

#include <osal/GUID.h>
#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>
#include "staruml/ClassDiagram.h"
#include "staruml/Model.h"

using namespace std;
using namespace StarUML;

Project::Project(const OSAL::GUID & id, const std::string & name)
    : Container(nullptr, "Project", id, name)
    , _models()
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
    OSAL::GUID id = OSAL::GUID::Generate();
    auto model = make_shared<Model>(shared_from_this(), id, "Model");
    AddElement(model);
    _models.push_back(model);
    id = OSAL::GUID::Generate();
    auto classDiagram = make_shared<ClassDiagram>(model, id, "Main");
    classDiagram->Visible(true);
    classDiagram->DefaultDiagram(true);
    model->AddElement(classDiagram);
}

Container::Ptr Project::GetFirstModel()
{
    if (_models.size() > 0)
        return _models[0];
    return nullptr;
}