#pragma once

#include "Object.h"

namespace StarUML {

class Container
    : public Object
{
public:
    Container() = delete;
    Container(Ptr parent, const std::string & type);
    Container(Ptr parent, const std::string & type, const OSAL::GUID & id);
    Container(Ptr parent, const std::string & type, const OSAL::GUID & id, const std::string & name);
    virtual ~Container();

    virtual void Serialize(std::ostream & stream) override;

    void AddElement(Object::Ptr element) { _ownedElements.push_back(element); }
    const Object::List & OwnedElements() { return _ownedElements; }

    virtual JSON::ValuePtr CreateObject() const;

protected:
    Object::List _ownedElements;
};

} // namespace StarUML
