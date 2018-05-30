#include "database/InsertBulkQuery.h"

using namespace std;
using namespace Database;

const string InsertString = "INSERT";
const string IntoString = "INTO";
const string ValuesString = "VALUES";

InsertBulkQuery::InsertBulkQuery() :
    CommandQuery()
{
}

InsertBulkQuery::~InsertBulkQuery()
{
}

void InsertBulkQuery::AddInsertInto(const string & tableName)
{
    insertClause = tableName;
}

void InsertBulkQuery::AddColumnName(const string & name)
{
    namesClause.push_back(name);
}

void InsertBulkQuery::AddColumnNames(vector<string> names)
{
    for (auto name : names)
    {
        AddColumnName(name);
    }
}

void InsertBulkQuery::AddValues(std::vector< std::string > values)
{
    string valuesString;
    for (auto value : values)
    {
        if (!valuesString.empty())
        {
            valuesString += ",";
        }
        valuesString += QuoteString(value);
    }
    valuesClause.push_back(valuesString);
}

string InsertBulkQuery::ToString() const
{
    string result;
    if (!insertClause.empty())
    {
        result += InsertString + " " + IntoString + " " + ConvertTableName(insertClause);
    }
    if (0 < namesClause.size())
    {
        result += "(";
        for (size_t i = 0; i < namesClause.size(); i++)
        {
            result += namesClause[i];
            if (namesClause.size() - 1 > i)
            {
                result += ",";
            }
        }
        result += ")";
    }
    if (0 < valuesClause.size())
    {
        result += " " + ValuesString + " ";
    }
    for (size_t i = 0; i < valuesClause.size(); i++)
    {
        result += "(" + valuesClause[i] + ")";
        if (valuesClause.size() - 1 > i)
        {
            result += ",";
        }
    }
    return result;
}
