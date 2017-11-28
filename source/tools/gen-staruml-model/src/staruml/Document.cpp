#include "staruml/Document.h"

using namespace StarUML;

Document::Document()
    : StarUML::Object()
{
    ObjectType("Project");
    ID("AAAAAAFF+h6SjaM2Hec=");
    Name("Untitled");
}

Document::~Document()
{

}

void Document::Serialize(std::ostream & stream)
{
    Object::Serialize(stream);
}