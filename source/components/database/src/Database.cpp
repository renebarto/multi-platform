#include "database/Database.h"

#include "database/CreateTableQuery.h"
#include "database/DeleteTableQuery.h"
#include "database/DeleteViewQuery.h"
#include "database/Exception.h"
#include "database/Table.h"

using namespace std;
namespace Database
{

static const string InformationSchema = "information_schema";
static const string SchemataTableName = "SCHEMATA";
static const string TablesTableName = "TABLES";
static const string ColumnsTableName = "COLUMNS";
static const string IndexesTableName = "STATISTICS";
static const string TableSchemaColumnName = "TABLE_SCHEMA";
static const string TableTypeColumnName = "TABLE_TYPE";
static const string TableNameColumnName = "TABLE_NAME";
static const string TablesTableTypeTable = "BASE TABLE";
static const string TablesTableTypeView = "VIEW";

Database::Database(DBEngine::Ptr engine,
                   const string & aSchemaName,
                   const string & aLocation,
                   const string & aUserID,
                   const string & aPassword) :
    engine(engine),
    connection(new Connection()),
    schemaName(aSchemaName),
    location(aLocation),
    userID(aUserID),
    password(aPassword)
{
    if (!Open(schemaName, location, userID, password))
        throw ConnectException(__func__, __FILE__, __LINE__, "Failed to open connection");
}

Database::~Database()
{
    if (IsOpen())
    {
        Close();
    }
    engine = nullptr;
}

bool Database::Open(const string & aSchemaName,
                    const string & aLocation,
                    const string & aUserID,
                    const string & aPassword)
{
    Close();

    schemaName = aSchemaName;
    location = DBEngine::GetLocationWithFallback(aLocation);
    userID = DBEngine::GetUserIDWithFallback(aUserID);
    password = DBEngine::GetPasswordWithFallback(aPassword);
    if (connection == nullptr)
        connection = ConnectionPtr(new Connection());
    if (connection->connected())
        connection->disconnect();
    return connection->connect(schemaName.empty() ? 0 : schemaName.c_str(),
                               location.empty() ? 0 : location.c_str(),
                               userID.empty() ? 0 : userID.c_str(),
                               password.empty() ? 0 : password.c_str());
}

bool Database::IsOpen()
{
    return (connection != nullptr) && connection->connected();
}

void Database::Close()
{
    if (IsOpen())
    {
        connection->disconnect();
    }
}

ConnectionPtr Database::GetConnection() const
{
    return connection;
}

DBEngine::Ptr Database::GetEngine() const
{
    return DBEngine::Ptr(engine);
}

string Database::GetLocation() const
{
    return location;
}

string Database::GetUserID() const
{
    return userID;
}

string Database::GetPassword() const
{
    return password;
}

ITable::Ptr Database::OpenTable(const string & tableName)
{
    ITable::Ptr table = make_shared<Table>(this, tableName);
    DataTable tableDefs = GetTableDefinitions(tableName);
    if (0 == tableDefs.num_rows())
    {
        return ITable::Ptr();
    }
    return table;
}

ITable::Ptr Database::CreateTable(const CreateTableQuery & defs)
{
    ITable::Ptr table = make_shared<Table>(this, defs);
    return table;
}

ITable::Ptr Database::OpenView(const string & viewName)
{
    ITable::Ptr table = make_shared<Table>(this, viewName);
    DataTable viewDefs = GetViewDefinitions(viewName);
    if (0 == viewDefs.num_rows())
    {
        return ITable::Ptr();
    }
    return table;
}

ITable::Ptr Database::CreateView(const CreateViewQuery & defs)
{
    ITable::Ptr table = make_shared<Table>(this, defs);
    return table;
}

bool Database::DeleteTable(const string & tableName)
{
    DeleteTableQuery query(tableName);
    try
    {
        DBEngine::ExecuteCommand(connection, query.ToString());
    }
    catch (const std::exception & e)
    {
        return false;
    }

    return true;
}

bool Database::DeleteView(const string & viewName)
{
    DeleteViewQuery query(viewName);
    try
    {
        DBEngine::ExecuteCommand(connection, query.ToString());
    }
    catch (const std::exception & e)
    {
        return false;
    }

    return true;
}

uint64_t Database::RunCommand(const string & command)
{
    return DBEngine::ExecuteCommand(connection, command);
}

uint64_t Database::RunCommand(const ::mysqlpp::Query & query)
{
    return DBEngine::ExecuteCommand(connection, const_cast<::mysqlpp::Query &>(query).str());
}

uint64_t Database::RunCommand(const CommandQuery & command)
{
    return DBEngine::ExecuteCommand(connection, command.ToString());
}

DataReader Database::RunQuery(const string & query)
{
    ::mysqlpp::Query mysqlQuery = connection->query(query);
    return DataReader(mysqlQuery.use());
}

DataReader Database::RunQuery(::mysqlpp::Query & query)
{
    return DataReader(query.use());
}

DataReader Database::RunQuery(const SelectQuery & query)
{
    ::mysqlpp::Query mysqlQuery = connection->query(query.ToString());
    return DataReader(mysqlQuery.use());
}

DataTable Database::ReadSingleTable(const SelectQuery & selectQuery)
{
    DataTable table;
    ::mysqlpp::Query query = connection->query(selectQuery.ToString());
    query.storein(table);
    table.SetTableName(selectQuery.FromClause());
    return table;
}

DataTable Database::ReadSingleTable(const string & tableName)
{
    SelectQuery query;
    query.AddSelect();
    query.AddFrom(tableName);
    return ReadSingleTable(query);
}

DataTable Database::GetTablesDefinitions()
{
    return GetTablesDefinitions(schemaName);
}

DataTable Database::GetTableDefinitions(const string & tableName)
{
    return GetTableDefinitions(schemaName, tableName);
}

DataTable Database::GetViewsDefinitions()
{
    return GetViewsDefinitions(schemaName);
}

DataTable Database::GetViewDefinitions(const string & viewName)
{
    return GetViewDefinitions(schemaName, viewName);
}

DataTable Database::GetTablesDefinitions(const string & schemaName)
{
    return GetTableViewDefinitions(TablesTableTypeTable, schemaName);
}

DataTable Database::GetTableDefinitions(const string & schemaName, const string & tableName)
{
    return GetTableViewDefinitions(TablesTableTypeTable, schemaName, tableName);
}

DataTable Database::GetViewsDefinitions(const string & schemaName)
{
    return GetTableViewDefinitions(TablesTableTypeView, schemaName);
}

DataTable Database::GetViewDefinitions(const string & schemaName, const string & viewName)
{
    return GetTableViewDefinitions(TablesTableTypeView, schemaName, viewName);
}

DataTable Database::GetColumnDefinitions(const string & tableName)
{
    return GetColumnDefinitions(schemaName, tableName);
}

DataTable Database::GetColumnDefinitions(const string & schemaName, const string & tableName)
{
    return GetDefinitions(ColumnsTableName, schemaName, tableName);
}

DataTable Database::GetIndexDefinitions(const string & tableName)
{
    return GetIndexDefinitions(schemaName, tableName);
}

DataTable Database::GetIndexDefinitions(const string & schemaName, const string & tableName)
{
    return GetDefinitions(IndexesTableName, schemaName, tableName);
}

DataTable Database::GetTableViewDefinitions(const string & tableType, const string & schemaName, const string & tableName)
{
    DataTable result;

    try
    {
        ConnectionPtr tempConnection = DBEngine::Connect(engine, InformationSchema, location, userID, password);
        if (tempConnection == nullptr)
            return DataTable();
        SelectQuery selectQuery;
        selectQuery.AddSelect();
        selectQuery.AddFrom(TablesTableName);
        if (!schemaName.empty())
            selectQuery.AddWhereString(TableSchemaColumnName, "=", schemaName);
        if (!tableName.empty())
            selectQuery.AddWhereString(TableNameColumnName, "=", tableName);
        selectQuery.AddWhereString(TableTypeColumnName, "=", tableType);
        ::mysqlpp::Query query = tempConnection->query(selectQuery.ToString());
        query.storein(result);
        tempConnection->disconnect();
    }
    catch (const ::mysqlpp::Exception & e)
    {
    }

    return result;
}

DataTable Database::GetDefinitions(const std::string & definitionsTableName,
                                   const string & schemaName,
                                   const string & tableName)
{
    DataTable result;

    try
    {
        ConnectionPtr tempConnection = DBEngine::Connect(engine, InformationSchema, location, userID, password);
        if (tempConnection == nullptr)
            return DataTable();
        SelectQuery selectQuery;
        selectQuery.AddSelect();
        selectQuery.AddFrom(definitionsTableName);
        if (!schemaName.empty())
            selectQuery.AddWhereString(TableSchemaColumnName, "=", schemaName);
        if (!tableName.empty())
            selectQuery.AddWhereString(TableNameColumnName, "=", tableName);
        ::mysqlpp::Query query = tempConnection->query(selectQuery.ToString());
        query.storein(result);
        tempConnection->disconnect();
    }
    catch (const ::mysqlpp::Exception & e)
    {
    }

    return result;
}

}

