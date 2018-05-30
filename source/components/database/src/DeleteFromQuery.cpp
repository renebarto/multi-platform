#include "database/DeleteFromQuery.h"

#include "core/serialization/SerializationImpl.h"

using namespace std;
using namespace Database;

const string DeleteString = "DELETE";
const string FromString = "FROM";
const string WhereString = "WHERE";
const string AndString = "AND";

DeleteFromQuery::DeleteFromQuery() :
    CommandQuery()
{
}

DeleteFromQuery::~DeleteFromQuery()
{
}

void DeleteFromQuery::AddFrom(const string & clause)
{
    fromClause.push_back(clause);
}
void DeleteFromQuery::AddWhereString(const string & operand1, const string & operatr, const string & operand2)
{
    whereClause.push_back(ConvertName(operand1) + operatr + QuoteString(operand2));
}
void DeleteFromQuery::AddWhere(const string & operand1, const string & operatr, const string & operand2)
{
    whereClause.push_back(ConvertName(operand1) + operatr + operand2);
}
void DeleteFromQuery::AddWhere(const string & operand1, const string & operatr, int operand2)
{
    whereClause.push_back(ConvertName(operand1) + operatr + Core::Serialize(operand2));
}
void DeleteFromQuery::AddWhere(const string & operand1, const string & operatr, uint operand2)
{
    whereClause.push_back(ConvertName(operand1) + operatr + Core::Serialize(operand2));
}
void DeleteFromQuery::AddWhere(const string & operand1, const string & operatr, double operand2)
{
    whereClause.push_back(ConvertName(operand1) + operatr + Core::Serialize(operand2));
}

string DeleteFromQuery::ToString() const
{
    string result;

    if (0 < fromClause.size())
    {
        result += DeleteString + " " + FromString + " ";
    }
    for (size_t i = 0; i < fromClause.size(); i++)
    {
        result += ConvertTableName(fromClause[i]);
        if (fromClause.size() - 1 > i)
        {
            result += ",";
        }
    }
    if (0 < whereClause.size())
    {
        result += " " + WhereString + " ";
    }
    for (size_t i = 0; i < whereClause.size(); i++)
    {
        result += "(" + whereClause[i] + ")";
        if (whereClause.size() - 1 > i)
        {
            result += " " + AndString + " ";
        }
    }
    return result;
}
