#include "database/InsertQuery.h"

using namespace std;
using namespace Database;

static const string InsertString = "INSERT";
static const string IntoString = "INTO";
static const string SetString = "SET";
static const string OnDuplicateSetString = "ON DUPLICATE KEY UPDATE";

InsertQuery::InsertQuery() :
    CommandQuery()
{
}

InsertQuery::~InsertQuery()
{
}

string InsertQuery::ToString() const
{
    string result;

    if (0 < insertClause.size())
    {
        result += InsertString + " " + IntoString + " ";
    }
    for (size_t i = 0; i < insertClause.size(); i++)
    {
        result += ConvertTableName(insertClause[i]);
        if (insertClause.size() - 1 > i)
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
    if (0 < onDuplicateSetClause.size())
    {
        result += " " + OnDuplicateSetString + " ";
    }
    for (size_t i = 0; i < onDuplicateSetClause.size(); i++)
    {
        result += onDuplicateSetClause[i];
        if (onDuplicateSetClause.size() - 1 > i)
        {
            result += ",";
        }
    }
    return result;
}
