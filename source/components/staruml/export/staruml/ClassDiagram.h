#pragma once

#include "Diagram.h"

namespace StarUML {

class ClassDiagram
    : public Diagram
{
public:
    ClassDiagram() = delete;
    explicit ClassDiagram(Ptr parent, const OSAL::ByteArray & id, const std::string & name);
    virtual ~ClassDiagram();

    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const override;
};

} // namespace StarUML
