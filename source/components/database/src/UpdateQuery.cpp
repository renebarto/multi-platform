#include "database/UpdateQuery.h"

using namespace std;
using namespace Database;

static const string UpdateString = "UPDATE";
static const string SetString = "SET";
static const string WhereString = "WHERE";

UpdateQuery::UpdateQuery() :
    CommandQuery()
{
}

UpdateQuery::~UpdateQuery()
{
}

string UpdateQuery::ToString() const
{
    string result;

    if (0 < updateClause.size())
    {
        result += UpdateString + " ";
    }
    for (size_t i = 0; i < updateClause.size(); i++)
    {
        result += ConvertTableName(updateClause[i]);
        if (updateClause.size() - 1 > i)
        {
            result += ",";
        }
    }
    if (0 < setClause.size())
    {
        result += " " + SetString + " ";
    }
    for (size_t i = 0; i < setClause.size(); i++)
    {
        result += setClause[i];
        if (setClause.size() - 1 > i)
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
            result += " AND ";
        }
    }
    return result;
}
