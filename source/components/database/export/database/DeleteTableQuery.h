#pragma once

#include "database/CommandQuery.h"

namespace Database
{

class DATABASE_EXPORT DeleteTableQuery: public CommandQuery
{
public:
    typedef std::shared_ptr<DeleteTableQuery> Ptr;

    explicit DeleteTableQuery(const std::string & tableName);
    virtual ~DeleteTableQuery();

    virtual std::string ToString() const;

private:
    std::string tableName;
};

} // namespace Database
