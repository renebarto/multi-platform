#pragma once

#include "database/CommandQuery.h"

namespace Database
{

class SelectQuery;

class DATABASE_EXPORT CreateViewQuery: public CommandQuery
{
public:
    typedef std::shared_ptr<CreateViewQuery> Ptr;

    explicit CreateViewQuery(const std::string & viewName);
    virtual ~CreateViewQuery();

    const std::string & GetViewName() const { return viewName; }
    const std::string & GetSelectQuery() const { return selectQuery; }
    void SetSelectQuery(const std::string & selectQuery) { this->selectQuery = selectQuery; }
    void SetSelectQuery(const SelectQuery & selectQuery);

    virtual std::string ToString() const;

private:
    std::string viewName;
    std::string selectQuery;
};

} // namespace Database
