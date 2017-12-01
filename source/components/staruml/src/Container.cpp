#include "../export/staruml/Container.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>

using namespace std;
using namespace StarUML;

Container::Container(Ptr parent, const std::string & type)
    : Object(parent, type)
    , _ownedElements()
{
}

Container::Container(Ptr parent, const std::string & type, const OSAL::ByteArray & id)
    : Object(parent, type, id)
    , _ownedElements()
{
}

Container::Container(Ptr parent, const std::string & type, const OSAL::ByteArray & id, const std::string & name)
    : Object(parent, type, id, name)
      , _ownedElements()
{
}

Container::~Container()
{

}

void Container::Serialize(std::ostream & stream)
{
    auto Container = CreateObject();
    Container->Serialize(stream, 0, 0);
}

JSON::ValuePtr Container::CreateObject() const
{
    auto result = dynamic_pointer_cast<JSON::Object>(Object::CreateObject());

    if (_ownedElements.size() > 0)
    {
        auto ownedElements = make_shared<JSON::Array>();
        result->AddPair(JSON::KVPair("ownedElements", ownedElements));
        for (auto element : _ownedElements)
        {
            ownedElements->AddValue(element->CreateObject());
        }
    }
    return result;
}
