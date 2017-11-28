#pragma once

#include <ostream>
#include <json/Object.h>

namespace StarUML {

class Object : protected JSON::Object
{
public:
    Object();
    virtual ~Object();

    virtual void Serialize(std::ostream & stream);

    std::string ObjectType() const
    {
        std::string value;
        if (!GetProperty("_type", value))
            return "";
        return value;
    }
    std::string ID() const
    {
        std::string value;
        if (!GetProperty("_id", value))
            return "";
        return value;
    }
    std::string Name() const
    {
        std::string value;
        if (!GetProperty("_name", value))
            return "";
        return value;
    }

protected:
    void ObjectType(const std::string & value) { SetProperty("_type", value); }
    void ID(const std::string & value) { SetProperty("_id", value); }
    void Name(const std::string & value) { SetProperty("_name", value); }

private:
    bool GetProperty(const std::string & name, std::string & value) const;
    void SetProperty(const std::string & name, const std::string & value);
};

} // namespace StarUML
