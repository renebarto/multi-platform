#include "staruml/Document.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>
#include <staruml/Diagram.h>
#include <staruml/Model.h>

using namespace std;
using namespace StarUML;

static OSAL::ByteArray ProjectId({0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0x1e,
                                  0x92, 0x8d, 0xa3, 0x36, 0x1d, 0xe7});

Document::Document()
    : Container("Project", ProjectId, "Untitled")
{
}

Document::~Document()
{

}

void Document::Serialize(std::ostream & stream)
{
    auto document = CreateObject();
    document->Serialize(stream, 0, 0);
}

JSON::ValuePtr Document::CreateObject() const
{
    auto result = dynamic_pointer_cast<JSON::Object>(Container::CreateObject());
    return result;
}

void Document::SetupDefaultProject()
{
    OSAL::ByteArray id = Core::Util::Base64Decode("AAAAAAFF+qBWK6M3Z8Y=");
    auto model = make_shared<Model>(id);
    AddElement(model);
    id = Core::Util::Base64Decode("AAAAAAFF+qBtyKM79qY=");
    model->AddElement(make_shared<Diagram>(id, "Main"));
}
