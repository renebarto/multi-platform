#pragma once

#include "Container.h"

namespace StarUML {

enum class ObjectVisibility
{
    Public,
    Protected,
    Private,
    Package
};
std::ostream & operator << (std::ostream & stream, const ObjectVisibility & value);

class Model
    : public Container
{
public:
    Model() = delete;
    explicit Model(Ptr parent, const OSAL::ByteArray & id, const std::string & name);
    virtual ~Model();

    ObjectVisibility Visibility() const { return _visibility; }
    void Visibility(ObjectVisibility value) { _visibility = value; }
    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const override;

private:
    ObjectVisibility _visibility;
};

} // namespace StarUML
