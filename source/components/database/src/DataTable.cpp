#include "database/DataTable.h"

#include "database/Exception.h"

using namespace std;
using namespace Database;

DataTable::DataTable()
{
}

DataTable::DataTable(const ::mysqlpp::StoreQueryResult & other) :
    ::mysqlpp::StoreQueryResult(other),
    tableName()
{
}

DataTable::DataTable(const DataTable & other) :
    ::mysqlpp::StoreQueryResult(other),
    tableName(other.tableName)
{
}

DataTable & DataTable::operator = (const ::mysqlpp::StoreQueryResult & other)
{
    if (this != &other)
    {
        ::mysqlpp::StoreQueryResult * base = dynamic_cast< ::mysqlpp::StoreQueryResult *>(this);
        *base = other;
        tableName = "";
    }
    return *this;
}

DataTable & DataTable::operator = (const DataTable & other)
{
    if (this != &other)
    {
        ::mysqlpp::StoreQueryResult * base = dynamic_cast< ::mysqlpp::StoreQueryResult *>(this);
        *base = other;
        tableName = other.tableName;
    }
    return *this;
}

const string & DataTable::GetTableName() const
{
    return tableName;
}

void DataTable::SetTableName(const string & tableName)
{
    this->tableName = tableName;
}

size_t DataTable::NumFields() const
{
    if (size() <= 0)
        return 0;
    const ::mysqlpp::Row & row = at(0);
    return row.size();
}

const string & DataTable::FieldName(size_t i) const
{
    if (size() <= 0)
        throw FieldIndexException(__func__, __FILE__, __LINE__, "DataTable::FieldName", i);
    const ::mysqlpp::Row & row = at(0);
    return row.field_list().list->at(i);
}

::mysqlpp::StoreQueryResult::list_type::size_type DataTable::NumRows() const
{
    return size();
}

DataRow DataTable::operator[] (size_t i) const
{
    if (i >= size())
        throw RowIndexException(__func__, __FILE__, __LINE__, "DataTable::FieldName", i);
    const ::mysqlpp::Row & row = at(i);
    return DataRow(row);
}
