#pragma once

#include <memory>
#include "database/DataTable.h"
#include "database/DBEngine.h"

namespace Database
{

class DATABASE_EXPORT ITable
{
public:
    typedef std::shared_ptr<ITable> Ptr;

    ITable() {}
    virtual ~ITable() {}

    virtual const std::string & GetTableName() = 0;

    virtual DataTable GetTableDefinitions() = 0;
    virtual DataTable GetColumnDefinitions() = 0;
    virtual DataTable GetIndexDefinitions() = 0;
};

} // namespace Database
