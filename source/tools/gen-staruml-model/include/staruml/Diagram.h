#pragma once

#include "staruml/Container.h"

namespace StarUML {

class Diagram : public Container
{
public:
    Diagram() = delete;
    explicit Diagram(const OSAL::ByteArray & id, const std::string & name);
    virtual ~Diagram();

    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const override;
};

} // namespace StarUML
