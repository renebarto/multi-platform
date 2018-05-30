#pragma once

#include "database/ITable.h"

namespace Database
{

class CreateTableQuery;
class CreateViewQuery;

class DATABASE_EXPORT Table : public ITable
{
public:
    typedef std::shared_ptr<Table> Ptr;

    Table(IDatabase * database, const std::string & tableName);
    Table(IDatabase * database, const CreateTableQuery & defs);
    Table(IDatabase * database, const CreateViewQuery & defs);
    virtual ~Table();

    const std::string & GetTableName() { return tableName; }

    DataTable GetTableDefinitions();
    DataTable GetColumnDefinitions();
    DataTable GetIndexDefinitions();

private:
    IDatabase * database;
    std::string tableName;
};

} // namespace Database
