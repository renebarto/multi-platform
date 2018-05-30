#pragma once

#include <mysql++/mysql++.h>
#include "database/exports.h"

namespace Database
{

class DATABASE_EXPORT DataRow : public ::mysqlpp::Row
{
public:
    DataRow();
    explicit DataRow(const ::mysqlpp::Row & other);
    DataRow(const DataRow & other);

    DataRow & operator = (const ::mysqlpp::Row & other);
    DataRow & operator = (const DataRow & other);

    bool IsEmpty() { return empty(); }
    bool operator ! () { return empty(); }

    const_reference operator [](const char * field) const;
    const_reference operator [](const std::string & field) const;
    const_reference operator [](int i) const;
};

} // DBSupport
