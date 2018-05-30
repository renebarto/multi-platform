#pragma once

#include <memory>
#include <string>
#include <mysql++/mysql++.h>
#include "database/DataReader.h"
#include "database/DataTable.h"

namespace Database
{

typedef ::mysqlpp::Connection Connection;
typedef std::shared_ptr<Connection> ConnectionPtr;

class IDatabase;
typedef std::shared_ptr<IDatabase> IDatabasePtr;

class ColumnDefinition;

class DATABASE_EXPORT DBEngine
{
public:
    typedef std::shared_ptr<DBEngine> Ptr;

    DBEngine();
    virtual ~DBEngine();

    static std::string GetDefaultLocation();
    static void SetDefaultLocation(const std::string & location);

    static std::string GetDefaultUserID();
    static void SetDefaultUserID(const std::string & userID);

    static std::string GetDefaultPassword();
    static void SetDefaultPassword(const std::string & password);

    static std::string QuoteString(const std::string & unquotedString);
    static std::string QuoteIdentifier(const std::string & name);
    static std::string ConvertTableName(const std::string & tableName);

    static bool BackupDatabase(Ptr engine,
                               const std::string & fileName,
                               const std::string & schemaName,
                               const std::string & location = std::string(),
                               const std::string & userID = std::string(),
                               const std::string & password = std::string());

    static bool BackupTable(Ptr engine,
                            const std::string & fileName,
                            const std::string & tableName,
                            const std::string & schemaName,
                            const std::string & location = std::string(),
                            const std::string & userID = std::string(),
                            const std::string & password = std::string());

    static bool RestoreDatabase(Ptr engine,
                                const std::string & fileName,
                                const std::string & schemaName,
                                const std::string & location = std::string(),
                                const std::string & userID = std::string(),
                                const std::string & password = std::string());

    static bool RestoreTable(Ptr engine,
                             const std::string & fileName,
                             const std::string & tableName,
                             const std::string & schemaName,
                             const std::string & location = std::string(),
                             const std::string & userID = std::string(),
                             const std::string & password = std::string());

    static IDatabasePtr OpenDatabase(Ptr engine,
                                     const std::string & schemaName,
                                     const std::string & location = std::string(),
                                     const std::string & userID = std::string(),
                                     const std::string & password = std::string());

    static bool CreateDatabase(Ptr engine,
                               const std::string & schemaName,
                               const std::string & location = std::string(),
                               const std::string & userID = std::string(),
                               const std::string & password = std::string());

    static IDatabasePtr CreateAndOpenDatabase(Ptr engine,
                                              const std::string & schemaName,
                                              const std::string & location = std::string(),
                                              const std::string & userID = std::string(),
                                              const std::string & password = std::string());

    static DataTable GetDatabasesDefinitions(Ptr engine,
                                             const std::string & schemaName,
                                             const std::string & location = std::string(),
                                             const std::string & userID = std::string(),
                                             const std::string & password = std::string());

    static bool DeleteDatabase(Ptr engine,
                               const std::string & schemaName,
                               const std::string & location = std::string(),
                               const std::string & userID = std::string(),
                               const std::string & password = std::string());

    static ConnectionPtr Connect(Ptr engine,
                                 const std::string & schemaName,
                                 const std::string & location = std::string(),
                                 const std::string & userID = std::string(),
                                 const std::string & password = std::string());

    static std::string GetTypeName(const ColumnDefinition & col);
    static std::string BuildColumnDefinitionString(const ColumnDefinition & col);

    static uint64_t ExecuteCommand(ConnectionPtr connection, const std::string & commandText);
    static DataReader ExecuteQuery(ConnectionPtr connection, const std::string & commandText);

    static std::string GetLocationWithFallback(const std::string & location);
    static std::string GetUserIDWithFallback(const std::string & userId);
    static std::string GetPasswordWithFallback(const std::string & password);

private:
    static std::string defaultLocation;
    static std::string defaultUserID;
    static std::string defaultPassword;
};

} // namespace Database

