#pragma once

#include <vector>
#include "core/serialization/SerializationImpl.h"
#include "database/CommandQuery.h"

namespace Database
{

class DATABASE_EXPORT InsertBulkQuery: public CommandQuery
{
public:
    typedef std::shared_ptr<InsertBulkQuery> Ptr;

    InsertBulkQuery();
    virtual ~InsertBulkQuery();

    void AddInsertInto(const std::string & tableName);
    void AddColumnName(const std::string &  name);
    void AddColumnNames(std::vector< std::string > names);

    template <class T>
    void AddValues(T value1);

    template <class T,class U>
    void AddValues(T value1, U value2);

    template <class T,class U,class V>
    void AddValues(T value1, U value2, V value3);

    template <class T,class U,class V,class W>
    void AddValues(T value1, U value2, V value3, W value4);

    template <class T,class U,class V,class W,class X>
    void AddValues(T value1, U value2, V value3, W value4, X value5);

    template <class T,class U,class V,class W,class X,class Y>
    void AddValues(T value1, U value2, V value3, W value4, X value5, Y value6);

    void AddValues(std::vector< std::string > values);

    virtual std::string ToString() const;

private:
    std::string insertClause;
    std::vector< std::string > namesClause;
    std::vector< std::string > valuesClause;
};

template <class T>
void InsertBulkQuery::AddValues(T value1)
{
    std::string valuesString = Core::Serialize(value1);
    valuesClause.push_back(valuesString);
}

template <class T,class U>
void InsertBulkQuery::AddValues(T value1, U value2)
{
    std::string valuesString;
    valuesString += Core::Serialize(value1);
    valuesString += ",";
    valuesString += Core::Serialize(value2);
    valuesClause.push_back(valuesString);
}

template <class T,class U,class V>
void InsertBulkQuery::AddValues(T value1, U value2, V value3)
{
    std::string valuesString;
    valuesString += Core::Serialize(value1);
    valuesString += ",";
    valuesString += Core::Serialize(value2);
    valuesString += ",";
    valuesString += Core::Serialize(value3);
    valuesClause.push_back(valuesString);
}

template <class T,class U,class V,class W>
void InsertBulkQuery::AddValues(T value1, U value2, V value3, W value4)
{
    std::string valuesString;
    valuesString += Core::Serialize(value1);
    valuesString += ",";
    valuesString += Core::Serialize(value2);
    valuesString += ",";
    valuesString += Core::Serialize(value3);
    valuesString += ",";
    valuesString += Core::Serialize(value4);
    valuesClause.push_back(valuesString);
}

template <class T,class U,class V,class W,class X>
void InsertBulkQuery::AddValues(T value1, U value2, V value3, W value4, X value5)
{
    std::string valuesString;
    valuesString += Core::Serialize(value1);
    valuesString += ",";
    valuesString += Core::Serialize(value2);
    valuesString += ",";
    valuesString += Core::Serialize(value3);
    valuesString += ",";
    valuesString += Core::Serialize(value4);
    valuesString += ",";
    valuesString += Core::Serialize(value5);
    valuesClause.push_back(valuesString);
}

template <class T,class U,class V,class W,class X,class Y>
void InsertBulkQuery::AddValues(T value1, U value2, V value3, W value4, X value5, Y value6)
{
    std::string valuesString;
    valuesString += Core::Serialize(value1);
    valuesString += ",";
    valuesString += Core::Serialize(value2);
    valuesString += ",";
    valuesString += Core::Serialize(value3);
    valuesString += ",";
    valuesString += Core::Serialize(value4);
    valuesString += ",";
    valuesString += Core::Serialize(value5);
    valuesString += ",";
    valuesString += Core::Serialize(value6);
    valuesClause.push_back(valuesString);
}

} // namespace Database
