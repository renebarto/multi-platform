#pragma once

#include "Container.h"

namespace StarUML {

class Class
    : public Container
{
public:
    Class() = delete;
    Class(Ptr parent);
    Class(Ptr parent, const OSAL::GUID & id);
    Class(Ptr parent, const OSAL::GUID & id, const std::string & name);
    virtual ~Class();

    ObjectVisibility Visibility() const { return _visibility; }
    void Visibility(ObjectVisibility value) { _visibility = value; }

    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const;

private:
    ObjectVisibility _visibility;
};

} // namespace StarUML
