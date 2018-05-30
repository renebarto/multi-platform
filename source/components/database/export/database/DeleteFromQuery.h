#pragma once

#include <vector>
#include "database/CommandQuery.h"

namespace Database
{

class DATABASE_EXPORT DeleteFromQuery: public CommandQuery
{
public:
    typedef std::shared_ptr<DeleteFromQuery> Ptr;

    DeleteFromQuery();
    virtual ~DeleteFromQuery();

    void AddFrom(const std::string & clause);
    void AddWhereString(const std::string & operand1, const std::string & operatr, const std::string & operand2);
    void AddWhere(const std::string & operand1, const std::string & operatr, const std::string & operand2);
    void AddWhere(const std::string & operand1, const std::string & operatr, int operand2);
    void AddWhere(const std::string & operand1, const std::string & operatr, uint operand2);
    void AddWhere(const std::string & operand1, const std::string & operatr, double operand2);

    virtual std::string ToString() const;

private:
    std::vector< std::string > fromClause;
    std::vector< std::string > whereClause;
};

} // namespace Database
