#pragma once

#include <mysql++/mysql++.h>
#include "database/DataRow.h"

namespace Database
{

class DATABASE_EXPORT DataTable : public ::mysqlpp::StoreQueryResult
{
public:
    DataTable();
    explicit DataTable(const ::mysqlpp::StoreQueryResult & other);
    DataTable(const DataTable & other);

    DataTable & operator = (const ::mysqlpp::StoreQueryResult & other);
    DataTable & operator = (const DataTable & other);

    const std::string & GetTableName() const;
    void SetTableName(const std::string & tableName);

    size_t NumFields() const;
    const std::string & FieldName(size_t i) const;

    list_type::size_type NumRows() const;

    DataRow operator[] (size_t i) const;

private:
    std::string tableName;
};

} // DBSupport
