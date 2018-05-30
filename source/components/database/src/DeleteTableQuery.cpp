#include "database/DeleteTableQuery.h"

using namespace std;
using namespace Database;

static const string DeleteTableString = "DROP TABLE";

DeleteTableQuery::DeleteTableQuery(const string & tableName) :
    CommandQuery(),
    tableName(tableName)
{
}

DeleteTableQuery::~DeleteTableQuery()
{
}

string DeleteTableQuery::ToString() const
{
    string result = DeleteTableString + " " + ConvertTableName(tableName) + ";";
    return result;
}
