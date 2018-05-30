#pragma once

#include "database/CommandQuery.h"

namespace Database
{

struct DATABASE_EXPORT TypeMapEntry
{
public:
    std::string type;
    std::string name;

    TypeMapEntry(const std::string & type, const std::string & name) :
        type(type),
        name(name)
    {
    }
};

class DATABASE_EXPORT ColumnDefinition
{
public:
    std::string columnName;
    std::string columnType;
    int size;
    bool nullable;
    bool uniqueKey;
    bool autoIncrement;

    ColumnDefinition(const std::string & columnName, const std::type_info & columnType, int size,
                     bool nullable, bool autoIncrement, bool uniqueKey) :
        columnName(columnName),
        columnType(columnType.name()),
        size(size),
        nullable(nullable),
        uniqueKey(uniqueKey),
        autoIncrement(autoIncrement)
    {
    }
    ColumnDefinition(const std::string & columnName, const std::string & columnType) :
        columnName(columnName),
        columnType(columnType),
        size(0),
        nullable(false),
        uniqueKey(false),
        autoIncrement(false)
    {
    }
};

} // namespace Database
