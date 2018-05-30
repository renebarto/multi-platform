#include "database/DeleteViewQuery.h"

using namespace std;
using namespace Database;

static const string DeleteViewString = "DROP VIEW";

DeleteViewQuery::DeleteViewQuery(const string & viewName) :
    CommandQuery(),
    viewName(viewName)
{
}

DeleteViewQuery::~DeleteViewQuery()
{
}

string DeleteViewQuery::ToString() const
{
    string result = DeleteViewString + " " + ConvertTableName(viewName) + ";";
    return result;
}
