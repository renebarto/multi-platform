#pragma once

#include "Container.h"

namespace StarUML {

class Package
    : public Container
{
public:
    Package() = delete;
    Package(Ptr parent);
    Package(Ptr parent, const OSAL::GUID & id);
    Package(Ptr parent, const OSAL::GUID & id, const std::string & name);
    virtual ~Package();

    ObjectVisibility Visibility() const { return _visibility; }
    void Visibility(ObjectVisibility value) { _visibility = value; }

    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const;

private:
    ObjectVisibility _visibility;
};

} // namespace StarUML
