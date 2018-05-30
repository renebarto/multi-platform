#pragma once

#include <mysql++/mysql++.h>
#include "database/DBEngine.h"
#include "database/ITable.h"
#include "database/SelectQuery.h"

namespace Database
{

class CommandQuery;
class CreateTableQuery;
class CreateViewQuery;
class SelectQuery;

class DATABASE_EXPORT IDatabase
{
public:
    typedef std::shared_ptr<IDatabase> Ptr;

    IDatabase() {}
    virtual ~IDatabase() {}

    virtual bool IsOpen() = 0;
    virtual void Close() = 0;

    virtual ConnectionPtr GetConnection() const = 0;
    virtual DBEngine::Ptr GetEngine() const = 0;

    virtual ITable::Ptr OpenTable(const std::string & tableName) = 0;
    virtual ITable::Ptr CreateTable(const CreateTableQuery & definitions) = 0;
    virtual ITable::Ptr OpenView(const std::string & viewName) = 0;
    virtual ITable::Ptr CreateView(const CreateViewQuery & definitions) = 0;
    virtual bool DeleteTable(const std::string & tableName) = 0;
    virtual bool DeleteView(const std::string & viewName) = 0;
    virtual uint64_t RunCommand(const std::string & command) = 0;
    virtual uint64_t RunCommand(const ::mysqlpp::Query & query) = 0;
    virtual uint64_t RunCommand(const CommandQuery & command) = 0;
    virtual DataReader RunQuery(const std::string & query) = 0;
    virtual DataReader RunQuery(::mysqlpp::Query & query) = 0;
    virtual DataReader RunQuery(const SelectQuery & query) = 0;

    virtual DataTable GetTablesDefinitions() = 0;
    virtual DataTable GetTableDefinitions(const std::string & tableName) = 0;
    virtual DataTable GetViewsDefinitions() = 0;
    virtual DataTable GetViewDefinitions(const std::string & viewName) = 0;
    virtual DataTable ReadSingleTable(const SelectQuery & selectQuery) = 0;
    virtual DataTable ReadSingleTable(const std::string & tableName) = 0;

    template <class T>
    void ReadSingleTable(const SelectQuery & selectQuery, std::set<T> & result)
    {
        ::mysqlpp::Query query = GetConnection()->query(selectQuery.ToString());
        query.storein(result);
    }
    template <class T>
    void ReadSingleTable(const std::string & tableName, std::set<T> & result)
    {
        SelectQuery query;
        query.AddSelect();
        query.AddFrom(tableName);
        ReadSingleTable(query, result);
    }

    virtual DataTable GetTablesDefinitions(const std::string & schemaName) = 0;
    virtual DataTable GetTableDefinitions(const std::string & schemaName,
                                          const std::string & tableName) = 0;
    virtual DataTable GetViewsDefinitions(const std::string & schemaName) = 0;
    virtual DataTable GetViewDefinitions(const std::string & schemaName,
                                         const std::string & viewName) = 0;
    virtual DataTable GetColumnDefinitions(const std::string & tableName) = 0;
    virtual DataTable GetColumnDefinitions(const std::string & schemaName,
                                           const std::string & tableName) = 0;
    virtual DataTable GetIndexDefinitions(const std::string & tableName) = 0;
    virtual DataTable GetIndexDefinitions(const std::string & schemaName,
                                          const std::string & tableName) = 0;
};

} // DBSupport
