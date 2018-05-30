#pragma once

#include <vector>
#include "core/serialization/SerializationImpl.h"
#include "database/CommandQuery.h"

namespace Database
{

class DATABASE_EXPORT UpdateQuery: public CommandQuery
{
public:
    typedef std::shared_ptr<UpdateQuery> Ptr;

    UpdateQuery();
    virtual ~UpdateQuery();

    void AddUpdate(const std::string & clause)
    {
        updateClause.push_back(clause);
    }
    void AddSetString(const std::string & operand1, const std::string & operand2)
    {
        setClause.push_back(ConvertName(operand1) + "=" + QuoteString(operand2));
    }
    void AddSet(const std::string & operand1, const std::string & operand2)
    {
        setClause.push_back(ConvertName(operand1) + "=" + operand2);
    }
    template <class T> void AddSet(const std::string & operand1, T operand2)
    {
        setClause.push_back(ConvertName(operand1) + "=" + Core::Serialize(operand2));
    }
    void AddWhereString(const std::string & operand1, const std::string & operatr, const std::string & operand2)
    {
        whereClause.push_back(ConvertName(operand1) + operatr + QuoteString(operand2));
    }
    void AddWhere(const std::string & operand1, const std::string & operatr, const std::string & operand2)
    {
        whereClause.push_back(ConvertName(operand1) + operatr + operand2);
    }
    template <class T> void AddWhere(const std::string & operand1, const std::string & operatr, T operand2)
    {
        whereClause.push_back(ConvertName(operand1) + operatr + Core::Serialize(operand2));
    }

    virtual std::string ToString() const;

private:
    std::vector< std::string > updateClause;
    std::vector< std::string > setClause;
    std::vector< std::string > whereClause;
};

} // namespace Database
