#include "settings/settings.h"

namespace Settings
{

settings::settings()
    : _settings(std::make_shared<JSON::Object>())
    , _isDirty()
{
}

settings::~settings()
{
}

bool settings::Load(std::basic_istream<char> & stream)
{
    JSON::Object loadedObject;
    auto result = loadedObject.Deserialize(stream);

    for (auto pair : loadedObject.GetValue())
    {
        const std::string & key = pair.first;
        JSON::ValuePtr value = pair.second;
        if (!Have(key))
        {
            Core::TraceWarning("Setting {} in loaded object not defined", key);
            return false;
        }
        _settings->Find(key)->Set(value);
    }
    _isDirty = false;
    return result;
}
void settings::Save(std::basic_ostream<char> & stream, int indentDepth, bool indentInitial) const
{
    _settings->Serialize(stream, indentDepth, indentInitial);
    _isDirty = false;
}

bool settings::Have(const std::string & key) const
{
    return _settings->Find(key) != nullptr;
}


} // namespace JSON
