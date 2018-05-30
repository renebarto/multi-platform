#include "database/DataReader.h"

#include "database/Exception.h"

using namespace std;
using namespace Database;

DataReader::DataReader()
{
}

DataReader::DataReader(const ::mysqlpp::UseQueryResult & other) :
    ::mysqlpp::UseQueryResult(other)
{
}

DataReader::DataReader(const DataReader & other) :
    ::mysqlpp::UseQueryResult(other)
{
}

DataReader & DataReader::operator = (const ::mysqlpp::UseQueryResult & other)
{
    if (this != &other)
    {
        ::mysqlpp::UseQueryResult * base = dynamic_cast< ::mysqlpp::UseQueryResult *>(this);
        *base = other;
    }
    return *this;
}

DataReader & DataReader::operator = (const DataReader & other)
{
    if (this != &other)
    {
        ::mysqlpp::UseQueryResult * base = dynamic_cast< ::mysqlpp::UseQueryResult *>(this);
        *base = other;
    }
    return *this;
}

size_t DataReader::NumFields() const
{
    return num_fields();
}

const std::string & DataReader::FieldName(size_t i) const
{
    return field_name(i);
}

const ::mysqlpp::FieldTypes::value_type & DataReader::FieldType(size_t i) const
{
    return field_type(i);
}

DataRow DataReader::NextRow() const
{
    return DataRow(fetch_row());
}
