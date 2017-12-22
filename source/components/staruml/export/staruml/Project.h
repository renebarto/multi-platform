#pragma once

#include "staruml/Container.h"

namespace StarUML {

class Model;

class Project
    : public Container
{
public:
    Project(const OSAL::GUID & id, const std::string & name);
    virtual ~Project();

    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const override;

    void SetupDefault();

    Container::Ptr GetFirstModel();

private:
    Container::List _models;
};

} // namespace StarUML
