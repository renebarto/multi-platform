#include "../export/staruml/Document.h"

#include <core/Util.h>
#include <json/String.h>
#include <json/Array.h>
#include "../export/staruml/Diagram.h"
#include "../export/staruml/Model.h"

using namespace std;
using namespace StarUML;

static OSAL::GUID ProjectId({0x00, 0x00, 0x00, 0x00, 0x01, 0x45, 0xfa, 0x1e,
                             0x92, 0x8d, 0xa3, 0x36, 0x1d, 0xe7, 0x00, 0x00});

Document::Document()
    : _project(make_shared<Project>(ProjectId, "Untitled"))
{
}

Document::~Document()
{

}

void Document::Serialize(std::ostream & stream)
{
    if (_project != nullptr)
        _project->Serialize(stream);
}

void Document::SetupDefaultProject()
{
    if (_project != nullptr)
        _project->SetupDefault();
}
