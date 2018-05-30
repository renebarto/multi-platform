#pragma once

#include <mysql++/mysql++.h>
#include "database/DataRow.h"

namespace Database
{

class DATABASE_EXPORT DataReader : public ::mysqlpp::UseQueryResult
{
public:
    DataReader();
    explicit DataReader(const ::mysqlpp::UseQueryResult & other);
    DataReader(const DataReader & other);

    DataReader & operator = (const ::mysqlpp::UseQueryResult & other);
    DataReader & operator = (const DataReader & other);

    size_t NumFields() const;
    const std::string & FieldName(size_t i) const;
    const ::mysqlpp::FieldTypes::value_type & FieldType(size_t i) const;

    DataRow NextRow() const;
};

} // DBSupport
