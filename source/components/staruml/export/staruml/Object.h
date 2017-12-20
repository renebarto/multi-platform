#pragma once

#include <ostream>
#include <osal/GUID.h>
#include <json/Object.h>

namespace StarUML {

enum class ObjectVisibility
{
    Public,
    Protected,
    Private,
    Package
};
std::ostream & operator << (std::ostream & stream, ObjectVisibility value);
inline std::string ToString(ObjectVisibility value)
{
    std::ostringstream stream;
    stream << value;
    return stream.str();
}

class Object
    : public std::enable_shared_from_this<Object>
{
public:
    using WeakPtr = std::weak_ptr<Object>;
    using Ptr = std::shared_ptr<Object>;
    using List = std::vector<Ptr>;

    Object() = delete;
    Object(Ptr parent, const std::string & type);
    Object(Ptr parent, const std::string & type, const OSAL::GUID & id);
    Object(Ptr parent, const std::string & type, const OSAL::GUID & id, const std::string & name);
    virtual ~Object();

    virtual void Serialize(std::ostream & stream);

    const std::string & Type() const { return _type; }
    void Type(const std::string & value) { _type = value; }
    const OSAL::GUID & Id() const { return _id; }
    void Id(const OSAL::ByteArray & value) { _id = value; }
    const std::string & Name() const { return _name; }
    void Name(const std::string & value) { _name = value; }
    Ptr Parent() const { return _parent.lock(); }
    void Parent(Ptr value) { _parent = value; }

    virtual JSON::ValuePtr CreateObject() const;

private:
    std::string _type;
    OSAL::GUID _id;
    std::string _name;
    WeakPtr _parent;

protected:
    std::string GetAttribute(JSON::ValuePtr object, const std::string & name) const;
    void SetAttribute(JSON::ValuePtr object, const std::string & name, const std::string & value) const;
};

} // namespace StarUML
