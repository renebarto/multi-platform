#include "staruml/Diagram.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>

using namespace std;
using namespace StarUML;

Diagram::Diagram(const OSAL::ByteArray & id, const std::string & name)
    : Container("UMLClassDiagram", id, name)
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
    return result;
}
