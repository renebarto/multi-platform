#include "database/CommandQuery.h"

#include "database/DBEngine.h"

using namespace Database;

CommandQuery::CommandQuery()
{
}

CommandQuery::~CommandQuery()
{
}

std::string CommandQuery::ConvertTableName(const std::string & tableName)
{
    return DBEngine::ConvertTableName(tableName);
}

std::string CommandQuery::ConvertName(const std::string & name)
{
    std::string result = name;
    if (name.find_first_of(' ') != std::string::npos)
    {
        result = DBEngine::QuoteIdentifier(name);
    }
    return result;
}

std::string CommandQuery::QuoteString(const std::string & text)
{
    return DBEngine::QuoteString(text);
}
