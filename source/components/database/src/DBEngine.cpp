#include "database/DBEngine.h"

#include <fstream>
#include "osal/Process.h"
#include "osal/Strings.h"
#include "database/ColumnDefinition.h"
#include "database/CreateTableQuery.h"
#include "database/Database.h"

using namespace std;
using namespace Database;

static const string InformationSchema = "information_schema";
static const string SchemataTableName = "SCHEMATA";

string DBEngine::defaultLocation;
string DBEngine::defaultUserID;
string DBEngine::defaultPassword;

DBEngine::DBEngine()
{
}

DBEngine::~DBEngine()
{
}

string DBEngine::GetDefaultLocation()
{
    return defaultLocation;
}

void DBEngine::SetDefaultLocation(const string & location)
{
    defaultLocation = location;
}

string DBEngine::GetDefaultUserID()
{
    return defaultUserID;
}

void DBEngine::SetDefaultUserID(const string & userID)
{
    defaultUserID = userID;
}

string DBEngine::GetDefaultPassword()
{
    return defaultPassword;
}

void DBEngine::SetDefaultPassword(const string & password)
{
    defaultPassword = password;
}

string DBEngine::QuoteString(const string & unquotedString)
{
    string result;
    if (unquotedString.find_first_of('"') != std::string::npos)
    {
        result = "'" + unquotedString + "'";
    }
    else
    {
        result = "\"" + unquotedString + "\"";
    }
    return result;
}

string DBEngine::QuoteIdentifier(const string & name)
{
    return "`" + name + "`";
}

string DBEngine::ConvertTableName(const string & tableName)
{
    string result = tableName;
    if (tableName.find_first_of(' ') != std::string::npos)
    {
        result = QuoteIdentifier(tableName);
    }
    return result;
}

bool DBEngine::BackupDatabase(DBEngine::Ptr engine,
                              const string & fileName,
                              const string & schemaName,
                              const string & location,
                              const string & userID,
                              const string & password)
{
    return BackupTable(engine, fileName, "", schemaName, location, userID, password);
}

bool DBEngine::BackupTable(DBEngine::Ptr engine __attribute__((unused)),
                           const string & fileName,
                           const string & tableName,
                           const string & schemaName,
                           const string & location,
                           const string & userID,
                           const string & password)
{
    OSAL::CommandArgs arguments;
    string theLocation = GetLocationWithFallback(location);
    string theUserID = GetUserIDWithFallback(userID);
    string thePassword = GetPasswordWithFallback(password);
    if (!theLocation.empty())
    {
        arguments.push_back("--host=" + theLocation);
    }
    if (!theUserID.empty())
    {
        arguments.push_back("--user=" + theUserID);
    }
    if (!thePassword.empty())
    {
        arguments.push_back("--password=" + thePassword);
    }
    if (!tableName.empty())
    {
        arguments.push_back(schemaName);
        arguments.push_back(tableName);
    }
    else
    {
        arguments.push_back("--databases");
        arguments.push_back(schemaName);
    }

    OSAL::Process process("mysqldump", arguments);
    process.Start();
    process.WaitForExit();
    bool result = (0 == process.ExitCode());
    string output;
    do
    {
        process.ReadStdOut();
        output += process.StdOut();
    }
    while (!process.StdOut().empty());
    ofstream fileStream(fileName);
    fileStream.write(output.c_str(), output.length());
    fileStream.close();
    return result;
}

bool DBEngine::RestoreDatabase(DBEngine::Ptr engine,
                               const string & fileName,
                               const string & schemaName,
                               const string & location,
                               const string & userID,
                               const string & password)
{
    return RestoreTable(engine, fileName, "", schemaName, location, userID, password);
}

bool DBEngine::RestoreTable(DBEngine::Ptr engine,
                            const string & fileName,
                            const string & tableName __attribute__((unused)),
                            const string & schemaName,
                            const string & location,
                            const string & userID,
                            const string & password)
{
    OSAL::CommandArgs arguments;
    string theLocation = GetLocationWithFallback(location);
    string theUserID = GetUserIDWithFallback(userID);
    string thePassword = GetPasswordWithFallback(password);
    if (!theLocation.empty())
    {
        arguments.push_back("--host=" + theLocation);
    }
    if (!theUserID.empty())
    {
        arguments.push_back("--user=" + theUserID);
    }
    if (!thePassword.empty())
    {
        arguments.push_back("--password=" + thePassword);
    }
    if (!schemaName.empty())
    {
        DeleteDatabase(engine, schemaName, theLocation, theUserID, thePassword);
        if (!CreateDatabase(engine, schemaName, theLocation, theUserID, thePassword))
            return false;
        arguments.push_back("--database=" + schemaName);
    }

    fstream fileStream(fileName, std::ios::openmode::_S_in);
    if (!fileStream)
        return false;
    OSAL::Process process("mysql", arguments);
    process.Start();
    process.RedirectStdIn(&fileStream);
    process.WriteStdIn();
    process.WaitForExit();
    fileStream.close();

    bool result = (0 == process.ExitCode());

    return result;
}

IDatabasePtr DBEngine::OpenDatabase(DBEngine::Ptr engine,
                                    const string & schemaName,
                                    const string & location,
                                    const string & userID,
                                    const string & password)
{
    IDatabase::Ptr database;

    try
    {
        string theLocation = GetLocationWithFallback(location);
        string theUserID = GetUserIDWithFallback(userID);
        string thePassword = GetPasswordWithFallback(password);
        database = IDatabasePtr(new Database(engine, schemaName, theLocation, theUserID, thePassword));
    }
    catch (const ::mysqlpp::Exception &)
    {
    }
    return database;
}

bool DBEngine::CreateDatabase(DBEngine::Ptr engine,
                              const string & schemaName,
                              const string & location,
                              const string & userID,
                              const string & password)
{
    bool result = false;
    try
    {
        ConnectionPtr connection = Connect(engine, "", location, userID, password);
        if (connection == nullptr)
            return false;
        result = connection->create_db(schemaName);
        connection->disconnect();
    }
    catch (const ::mysqlpp::Exception &)
    {
        result = false;
    }
    return result;
}

IDatabase::Ptr DBEngine::CreateAndOpenDatabase(DBEngine::Ptr engine,
                                             const string & schemaName,
                                             const string & location,
                                             const string & userID,
                                             const string & password)
{
    IDatabasePtr database;

    try
    {
        if (!CreateDatabase(engine, schemaName, location, userID, password))
            return IDatabasePtr();
        database = OpenDatabase(engine, schemaName, location, userID, password);
    }
    catch (const ::mysqlpp::Exception &)
    {
    }
    return database;
}

DataTable DBEngine::GetDatabasesDefinitions(DBEngine::Ptr engine,
                                            const string & schemaName __attribute__((unused)),
                                            const string & location,
                                            const string & userID,
                                            const string & password)
{
    DataTable result;

    try
    {
        ConnectionPtr connection = Connect(engine, InformationSchema, location, userID, password);
        if (connection == nullptr)
            return DataTable();
        SelectQuery selectQuery;
        selectQuery.AddSelect();
        selectQuery.AddFrom(SchemataTableName);
        ::mysqlpp::Query query = connection->query(selectQuery.ToString());
        query.storein(result);
        connection->disconnect();
    }
    catch (const ::mysqlpp::Exception & e)
    {
    }

    return result;
}

bool DBEngine::DeleteDatabase(DBEngine::Ptr engine,
                              const string & schemaName,
                              const string & location,
                              const string & userID,
                              const string & password)
{
    bool result = false;

    try
    {
        ConnectionPtr connection = Connect(engine, "", location, userID, password);
        if (connection == nullptr)
            return false;
        result = connection->drop_db(schemaName);
        connection->disconnect();
    }
    catch (const ::mysqlpp::Exception & e)
    {
        result = false;
    }

    return result;
}

ConnectionPtr DBEngine::Connect(DBEngine::Ptr engine __attribute__((unused)),
                                const string & schemaName,
                                const string & location,
                                const string & userID,
                                const string & password)
{
    string theLocation = GetLocationWithFallback(location);
    string theUserID = GetUserIDWithFallback(userID);
    string thePassword = GetPasswordWithFallback(password);
    ConnectionPtr connection = std::make_shared<Connection>();
    if (!connection->connect(schemaName.empty() ? 0 : schemaName.c_str(),
                             theLocation.empty() ? 0 : theLocation.c_str(),
                             theUserID.empty() ? 0 : theUserID.c_str(),
                             thePassword.empty() ? 0 : thePassword.c_str()))
    {
        connection.reset();
    }
    return connection;
}

string DBEngine::GetLocationWithFallback(const string & location)
{
    return (location.empty()) ? defaultLocation : location;
}

string DBEngine::GetUserIDWithFallback(const string & userId)
{
    return (userId.empty()) ? defaultUserID : userId;
}

string DBEngine::GetPasswordWithFallback(const string & password)
{
    return (password.empty()) ? defaultPassword : password;
}

static vector<TypeMapEntry> typeMapping =
{
    TypeMapEntry("j", "int(11) unsigned"),
    TypeMapEntry("i", "int(11)"),
    TypeMapEntry("Ss", "text"),
    TypeMapEntry("d", "double"),
    TypeMapEntry("f", "float"),
    TypeMapEntry("System.DateTime", "datetime"),
    TypeMapEntry("b", "tinyint(1)")
};

string DBEngine::GetTypeName(const ColumnDefinition & col)
{
    string convertedTypeName = "?";
    string typeName = col.columnType;

    for (size_t i = 0; i < typeMapping.size(); i++)
    {
        if (OSAL::Strings::IsEqualIgnoreCase(typeName, typeMapping[i].type))
        {
            convertedTypeName = typeMapping[i].name;
            break;
        }
    }
    return convertedTypeName;
}

string DBEngine::BuildColumnDefinitionString(const ColumnDefinition & col)
{
    string result = "";
    string typeName = GetTypeName(col);

    result += QuoteIdentifier(col.columnName) + " " + typeName;
    if (0 != col.size)
    {
        result += "(" + Core::Serialize(col.size) + ")";
    }
    if (!col.nullable)
    {
        result += " " + CreateTableQuery::NotNullString;
    }
    if (col.uniqueKey)
    {
        result += " " + CreateTableQuery::UniqueKeyString;
    }
    if (col.autoIncrement)
    {
        result += " " + CreateTableQuery::AutoIncrementString;
    }
    result += ",";
    return result;
}

uint64_t DBEngine::ExecuteCommand(ConnectionPtr connection, const std::string & commandText)
{
    ::mysqlpp::Query query = connection->query();
    ::mysqlpp::SimpleResult result = query.execute(commandText.c_str(), commandText.length());
    return result.rows();
}

DataReader DBEngine::ExecuteQuery(ConnectionPtr connection, const std::string & commandText)
{
    ::mysqlpp::Query query = connection->query(commandText);
    return DataReader(query.use());
}
