#pragma once

#include <ostream>
#include "staruml/Object.h"

namespace StarUML {

class Document : public Object
{
public:
    Document();
    virtual ~Document();

    virtual void Serialize(std::ostream & stream) override;
};

} // namespace StarUML
