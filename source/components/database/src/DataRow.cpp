#include "database/DataRow.h"

using namespace std;
using namespace Database;

DataRow::DataRow()
{

}

DataRow::DataRow(const ::mysqlpp::Row & other) :
    ::mysqlpp::Row(other)
{
}

DataRow::DataRow(const DataRow & other) :
    ::mysqlpp::Row(other)
{
}

DataRow & DataRow::operator = (const ::mysqlpp::Row & other)
{
    if (this != &other)
    {
        ::mysqlpp::Row * base = dynamic_cast< ::mysqlpp::Row *>(this);
        *base = other;
    }
    return *this;
}

DataRow & DataRow::operator = (const DataRow & other)
{
    if (this != &other)
    {
        ::mysqlpp::Row * base = dynamic_cast< ::mysqlpp::Row *>(this);
        *base = other;
    }
    return *this;
}

::mysqlpp::Row::const_reference DataRow::operator [](const char * field) const
{
    return ::mysqlpp::Row::operator [](field);
}

::mysqlpp::Row::const_reference DataRow::operator [](const string & field) const
{
    return ::mysqlpp::Row::operator [](field.c_str());
}

::mysqlpp::Row::const_reference DataRow::operator [](int i) const
{
    return ::mysqlpp::Row::operator [](i);
}
