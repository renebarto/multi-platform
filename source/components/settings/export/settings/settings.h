#pragma once

#include "settings/exports.h"
#include <iostream>
#include <core/Trace.h>
#include <json/Object.h>
#include <json/Number.h>
#include <json/String.h>
#include <json/Array.h>
#include <json/Boolean.h>

namespace Settings {

class settings
{
public:
    settings();
    virtual ~settings();

    bool Load(std::basic_istream<char> & stream);
    void Save(std::basic_ostream<char> & stream, int indentDepth = 0, bool indentInitial = true) const;

    template <class Elem, class Traits>
    std::basic_ostream<Elem, Traits> & PrintTo(std::basic_ostream<Elem, Traits> & s) const
    {
        auto dirty = _isDirty;
        Save(s, 0);
        _isDirty = dirty;
        return s;
    }

    bool Have(const std::string & key) const;
    bool Add(const std::string & key, JSON::ValuePtr value)
    {
        if (Have(key))
        {
            Core::TraceWarning("Setting {} is already defined", key);
            return false;
        }
        _settings->AddPair(JSON::KVPair(key, value));
        return true;
    }
    JSON::ValuePtr Get(const std::string & key) { return _settings->Find(key); }
    bool operator == (const settings & other) const
    {
        return _settings->EqualTo(other._settings);
    }
    bool operator != (const settings & other) const { return ! operator == (other); }

protected:
    std::shared_ptr<JSON::Object> _settings;
    mutable bool _isDirty;
};

template <class Elem, class Traits>
inline void PrintTo(std::basic_ostream<Elem, Traits> & stream, const settings & value)
{
    value.PrintTo(stream);
}

template <class Elem, class Traits>
inline std::basic_ostream<Elem, Traits> & operator << (std::basic_ostream<Elem, Traits> & stream, const settings & value)
{
    value.PrintTo(stream);
    return stream;
}

} // namepsace JSON
