#include "../export/staruml/Diagram.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>
#include <json/Boolean.h>

using namespace std;
using namespace StarUML;

Diagram::Diagram(Ptr parent, const std::string & type, const OSAL::ByteArray & id, const std::string & name)
    : Container(parent, type, id, name)
    , _visible(false)
    , _defaultDiagram(false)
{
}

Diagram::~Diagram()
{

}

void Diagram::Serialize(std::ostream & stream)
{
    auto Diagram = CreateObject();
    Diagram->Serialize(stream, 0, 0);
}

JSON::ValuePtr Diagram::CreateObject() const
{
    auto result = dynamic_pointer_cast<JSON::Object>(Container::CreateObject());
    result->AddPair(JSON::KVPair("visible", make_shared<JSON::Boolean>(Visible())));
    result->AddPair(JSON::KVPair("defaultDiagram", make_shared<JSON::Boolean>(DefaultDiagram())));
    return result;
}
