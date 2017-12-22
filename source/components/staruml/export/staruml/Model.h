#pragma once

#include "Container.h"

namespace StarUML {

class Model
    : public Container
{
public:
    Model() = delete;
    explicit Model(Object::Ptr parent, const OSAL::GUID & id, const std::string & name);
    virtual ~Model();

    ObjectVisibility Visibility() const { return _visibility; }
    void Visibility(ObjectVisibility value) { _visibility = value; }

    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const override;

private:
    ObjectVisibility _visibility;
};

} // namespace StarUML
