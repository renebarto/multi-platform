#include "database/CreateViewQuery.h"

#include "database/SelectQuery.h"

using namespace std;
using namespace Database;

const string CreateViewString = "CREATE VIEW";
const string AsString = "AS";

CreateViewQuery::CreateViewQuery(const string & viewName) :
    CommandQuery(),
    viewName(viewName)
{
}

CreateViewQuery::~CreateViewQuery()
{
}

void CreateViewQuery::SetSelectQuery(const SelectQuery & selectQuery)
{
    this->selectQuery = selectQuery.ToString();
}

string CreateViewQuery::ToString() const
{
    string result = CreateViewString + " " + ConvertTableName((viewName.empty()) ? "" : viewName) + " " +
           AsString + " " + selectQuery;
    return result;
}
