#include "../export/staruml/ClassDiagram.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>

using namespace std;
using namespace StarUML;

ClassDiagram::ClassDiagram(Ptr parent, const OSAL::GUID & id, const std::string & name)
    : Diagram(parent, "UMLClassDiagram", id, name)
{
}

ClassDiagram::~ClassDiagram()
{

}

void ClassDiagram::Serialize(std::ostream & stream)
{
    auto ClassDiagram = CreateObject();
    ClassDiagram->Serialize(stream, 0, 0);
}

JSON::ValuePtr ClassDiagram::CreateObject() const
{
    auto result = dynamic_pointer_cast<JSON::Object>(Diagram::CreateObject());
    return result;
}
