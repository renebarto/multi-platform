#include "database/CreateTableQuery.h"

#include "database/DBEngine.h"

using namespace std;
using namespace Database;

const string CreateTableQuery::CreateTableString = "CREATE TABLE";
const string CreateTableQuery::NotNullString = "NOT NULL";
const string CreateTableQuery::UniqueKeyString = "PRIMARY KEY";
const string CreateTableQuery::AutoIncrementString = "auto_increment";

CreateTableQuery::CreateTableQuery(const string & tableName) :
    CommandQuery(),
    tableName(tableName)
{
}

CreateTableQuery::~CreateTableQuery()
{
}

const std::string & CreateTableQuery::GetTableName() const
{
    return tableName;
}

void CreateTableQuery::AddColumn(const std::string & columnName, const std::type_info & columnType)
{
    AddColumn(columnName, columnType, 0);
}

void CreateTableQuery::AddColumn(const std::string & columnName, const std::type_info & columnType, int size)
{
    AddColumn(columnName, columnType, size, true);
}

void CreateTableQuery::AddColumn(const std::string & columnName, const std::type_info & columnType, int size, bool nullable)
{
    AddColumn(columnName, columnType, size, nullable, false);
}

void CreateTableQuery::AddColumn(const std::string & columnName, const std::type_info & columnType, int size, bool nullable, bool autoIncrement)
{
    AddColumn(columnName, columnType, size, nullable, autoIncrement, false);
}

void CreateTableQuery::AddColumn(const std::string & columnName, const std::type_info & columnType, int size, bool nullable, bool autoIncrement, bool uniqueKey)
{
    columns.push_back(ColumnDefinition(columnName, columnType, size, nullable, autoIncrement, uniqueKey));
}

void CreateTableQuery::AddColumn(const std::string & columnName, const std::string & columnType)
{
    columns.push_back(ColumnDefinition(columnName, columnType));
}

string CreateTableQuery::ToString() const
{
    string result = CreateTableString + " " + ConvertTableName((tableName.empty()) ? "" : tableName) + " (";
    if (0 != columns.size())
    {
        for (auto const & col : columns)
        {
            result += DBEngine::BuildColumnDefinitionString(col);
        }
        result = result.substr(0, result.length() - 1);
    }
    result += ");";
    return result;
}
