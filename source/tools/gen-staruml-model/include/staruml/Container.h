#pragma once

#include "staruml/Object.h"

namespace StarUML {

class Container : public Object
{
public:
    Container() = delete;
    Container(const std::string & type);
    Container(const std::string & type, const OSAL::ByteArray & id);
    Container(const std::string & type, const OSAL::ByteArray & id, const std::string & name);
    virtual ~Container();

    virtual void Serialize(std::ostream & stream) override;

    void AddElement(ObjectPtr element) { _ownedElements.push_back(element); }
    const ObjectList & OwnedElements() { return _ownedElements; }

    virtual JSON::ValuePtr CreateObject() const;

protected:
    ObjectList _ownedElements;
};

} // namespace StarUML
