#pragma once

#include "staruml/Container.h"

namespace StarUML {

class Model : public Container
{
public:
    Model() = delete;
    explicit Model(const OSAL::ByteArray & id);
    virtual ~Model();

    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const override;
};

} // namespace StarUML
