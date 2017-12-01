#pragma once

#include "Container.h"

namespace StarUML {

class Diagram
    : public Container
{
public:
    Diagram() = delete;
    explicit Diagram(Ptr parent, const std::string & type, const OSAL::ByteArray & id, const std::string & name);
    virtual ~Diagram();

    bool Visible() const { return _visible; }
    void Visible(bool value) { _visible = value; }
    bool DefaultDiagram() const { return _defaultDiagram; }
    void DefaultDiagram(bool value) { _defaultDiagram = value; }

    virtual void Serialize(std::ostream & stream) override;

    virtual JSON::ValuePtr CreateObject() const override;

private:
    bool _visible;
    bool _defaultDiagram;
};

} // namespace StarUML
