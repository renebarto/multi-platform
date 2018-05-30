#pragma once

#include "database/CommandQuery.h"

namespace Database
{

class DATABASE_EXPORT DeleteViewQuery: public CommandQuery
{
public:
    typedef std::shared_ptr<DeleteViewQuery> Ptr;

    explicit DeleteViewQuery(const std::string & viewName);
    virtual ~DeleteViewQuery();

    virtual std::string ToString() const;

private:
    std::string viewName;
};

} // namespace Database
