#pragma once

#include "database/IDatabase.h"

namespace Database
{

class DATABASE_EXPORT Database : public IDatabase
{
public:
    Database() = delete;
    Database(const Database &) = delete;
    Database(DBEngine::Ptr engine,
             const std::string & aSchemaName,
             const std::string & aLocation = std::string(),
             const std::string & aUserID = std::string(),
             const std::string & aPassword = std::string());
    virtual ~Database();

    Database & operator = (const Database &) = delete;

    bool Open(const std::string & aSchemaName,
              const std::string & aLocation = std::string(),
              const std::string & aUserID = std::string(),
              const std::string & aPassword = std::string());
    bool IsOpen();
    void Close();

    ConnectionPtr GetConnection() const;
    DBEngine::Ptr GetEngine() const;
    std::string GetLocation() const;
    std::string GetUserID() const;
    std::string GetPassword() const;

    ITable::Ptr OpenTable(const std::string & tableName);
    ITable::Ptr CreateTable(const CreateTableQuery & defs);
    ITable::Ptr OpenView(const std::string & viewName);
    ITable::Ptr CreateView(const CreateViewQuery & defs);
    bool DeleteTable(const std::string & tableName);
    bool DeleteView(const std::string & viewName);
    uint64_t RunCommand(const std::string & command);
    uint64_t RunCommand(const ::mysqlpp::Query & query);
    uint64_t RunCommand(const CommandQuery & command);
    DataReader RunQuery(const std::string & query);
    DataReader RunQuery(::mysqlpp::Query & query);
    DataReader RunQuery(const SelectQuery & query);

    DataTable GetTablesDefinitions();
    DataTable GetTableDefinitions(const std::string & tableName);
    DataTable GetViewsDefinitions();
    DataTable GetViewDefinitions(const std::string & viewName);
    DataTable ReadSingleTable(const SelectQuery & selectQuery);
    DataTable ReadSingleTable(const std::string & tableName);

    DataTable GetTablesDefinitions(const std::string & schemaName);
    DataTable GetTableDefinitions(const std::string & schemaName,
                                  const std::string & tableName);
    DataTable GetViewsDefinitions(const std::string & schemaName);
    DataTable GetViewDefinitions(const std::string & schemaName,
                                 const std::string & viewName);
    DataTable GetColumnDefinitions(const std::string & tableName);
    DataTable GetColumnDefinitions(const std::string & schemaName,
                                   const std::string & tableName);
    DataTable GetIndexDefinitions(const std::string & tableName);
    DataTable GetIndexDefinitions(const std::string & schemaName,
                                  const std::string & tableName);

private:
    DBEngine::Ptr engine;
    ConnectionPtr connection;
    std::string schemaName;
    std::string location;
    std::string userID;
    std::string password;
    DataTable GetTableViewDefinitions(const std::string & tableType,
                                      const std::string & schemaName,
                                      const std::string & tableName = std::string());
    DataTable GetDefinitions(const std::string & definitionsTableName,
                             const std::string & schemaName,
                             const std::string & tableName = std::string());
};

} // DBSupport
