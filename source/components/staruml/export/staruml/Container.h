#pragma once

#include "Object.h"

namespace StarUML {

class Container
    : public Object
{
public:
    using Ptr = std::shared_ptr<Container>;
    using List = std::vector<Ptr>;

    Container() = delete;
    Container(Object::Ptr parent, const std::string & type);
    Container(Object::Ptr parent, const std::string & type, const OSAL::GUID & id);
    Container(Object::Ptr parent, const std::string & type, const OSAL::GUID & id, const std::string & name);
    virtual ~Container();

    virtual void Serialize(std::ostream & stream) override;

    void AddElement(Object::Ptr element) { _ownedElements.push_back(element); }
    const Object::List & OwnedElements() { return _ownedElements; }

    virtual JSON::ValuePtr CreateObject() const;

protected:
    Object::List _ownedElements;
};

} // namespace StarUML
