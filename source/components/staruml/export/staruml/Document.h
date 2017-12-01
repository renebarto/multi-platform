#pragma once

#include "Project.h"

namespace StarUML {

class Document
{
public:
    Document();
    virtual ~Document();

    const std::string & Name() const { assert(_project != nullptr); return _project->Name(); }
    void Name(const std::string & value) { assert(_project != nullptr); _project->Name(value); }

    void Serialize(std::ostream & stream);

    void SetupDefaultProject();

private:
    std::shared_ptr<Project> _project;
};

} // namespace StarUML
