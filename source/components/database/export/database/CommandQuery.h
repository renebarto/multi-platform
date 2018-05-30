#pragma once

#include <memory>
#include "database/exports.h"

namespace Database
{

class DATABASE_EXPORT CommandQuery
{
public:
    typedef std::shared_ptr<CommandQuery> Ptr;

    CommandQuery();
    virtual ~CommandQuery();

    static std::string ConvertTableName(const std::string & tableName);
    static std::string ConvertName(const std::string & name);
    static std::string QuoteString(const std::string & text);

    virtual std::string ToString() const = 0;
};

} // namespace Database
