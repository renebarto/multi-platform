#pragma once

#include "Container.h"

namespace StarUML {

class Project
    : public Container
{
public:
    Project(const OSAL::GUID & id, const std::string & name);
    virtual ~Project();

    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const override;

    void SetupDefault();
};

} // namespace StarUML
