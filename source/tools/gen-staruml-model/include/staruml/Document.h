#pragma once

#include "staruml/Container.h"

namespace StarUML {

class Document : public Container
{
public:
    Document();
    virtual ~Document();

    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const override;

    void SetupDefaultProject();
};

} // namespace StarUML
