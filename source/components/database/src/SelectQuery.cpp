#include "database/SelectQuery.h"

using namespace std;
using namespace Database;

const string SelectQuery::SelectAll = "*";
static const string SelectString = "SELECT";
static const string FromString = "FROM";
static const string WhereString = "WHERE";
static const string OrderString = "ORDER BY";
static const string DistinctString = "DISTINCT";
static const string ContainsString = "INSTR";
static const string AsString = "AS";

SelectQuery::SelectQuery() :
    CommandQuery(),
    selectClause(),
    fromClause(),
    whereClause(),
    orderClause(),
    distinct(false)
{
}

SelectQuery::~SelectQuery()
{
}

void SelectQuery::AddSelectAs(const string & clause, const string & alias)
{
    string convertedClause = ConvertName(clause);
    if (!alias.empty())
    {
        convertedClause += ' ' + AsString + ' ' + ConvertName(alias);
    }
    selectClause.push_back(convertedClause);
}

void SelectQuery::AddFrom(const string & clause, const string & alias)
{
    string convertedClause = ConvertTableName(clause);
    if (!alias.empty())
    {
        convertedClause += ' ' + ConvertName(alias);
    }
    fromClause.push_back(convertedClause);
}

void SelectQuery::AddWhereContains(const std::string & column, const std::string & subString)
{
    whereClause.push_back(ContainsString + "(" + ConvertName(column) + "," + QuoteString(subString) + ")");
}

string SelectQuery::SelectClause() const
{
    string result = "";
    for (size_t i = 0; i < selectClause.size(); i++)
    {
        result += selectClause[i];
        if (selectClause.size() - 1 > i)
        {
            result += ",";
        }
    }
    return result;
}

string SelectQuery::FromClause() const
{
    string result = "";
    for (size_t i = 0; i < fromClause.size(); i++)
    {
        result += fromClause[i];
        if (fromClause.size() - 1 > i)
        {
            result += ",";
        }
    }
    return result;
}

string SelectQuery::WhereClause() const
{
    string result = "";
    for (size_t i = 0; i < whereClause.size(); i++)
    {
        result += "(" + whereClause[i] + ")";
        if (whereClause.size() - 1 > i)
        {
            result += " AND ";
        }
    }
    return result;
}

string SelectQuery::OrderClause() const
{
    string result = "";
    for (size_t i = 0; i < orderClause.size(); i++)
    {
        result += ConvertName(orderClause[i]);
        if (orderClause.size() - 1 > i)
        {
            result += ",";
        }
    }
    return result;
}

string SelectQuery::ToString() const
{
    string result = "";

    if (0 < selectClause.size())
    {
        result = SelectString + " ";
        if (distinct)
        {
            result += DistinctString + " ";
        }
        result += SelectClause();
    }
    if (0 < fromClause.size())
    {
        result += " " + FromString + " " + FromClause();
    }
    if (0 < whereClause.size())
    {
        result += " " + WhereString + " " + WhereClause();
    }
    if (0 < orderClause.size())
    {
        result += " " + OrderString + " " + OrderClause();
    }
    return result;
}
