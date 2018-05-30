#include "database/Table.h"

#include "database/CreateTableQuery.h"
#include "database/CreateViewQuery.h"
#include "database/IDatabase.h"

using namespace std;
using namespace Database;

Table::Table(IDatabase * database, const std::string & tableName) :
    database(database),
    tableName(tableName)
{
}

Table::Table(IDatabase * database, const CreateTableQuery & defs) :
    database(database),
    tableName(defs.GetTableName())
{
    DBEngine::ExecuteCommand(database->GetConnection(), defs.ToString());
}

Table::Table(IDatabase * database, const CreateViewQuery & defs) :
    database(database),
    tableName(defs.GetViewName())
{
    DBEngine::ExecuteCommand(database->GetConnection(), defs.ToString());
}

Table::~Table()
{

}

DataTable Table::GetTableDefinitions()
{
    return database->GetTableDefinitions(tableName);
}

DataTable Table::GetColumnDefinitions()
{
    return database->GetColumnDefinitions(tableName);
}

DataTable Table::GetIndexDefinitions()
{
    return database->GetIndexDefinitions(tableName);
}
