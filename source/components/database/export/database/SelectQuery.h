#pragma once

#include <vector>
#include "core/serialization/SerializationImpl.h"
#include "database/CommandQuery.h"

namespace DATABASE_EXPORT Database
{

class SelectQuery: public CommandQuery
{
public:
    typedef std::shared_ptr<SelectQuery> Ptr;

    static const std::string SelectAll;

    SelectQuery();
    virtual ~SelectQuery();

    void AddSelect(const std::string & clause = SelectAll)
    {
        AddSelectAs(clause);
    }
    void AddSelectAs(const std::string & clause, const std::string & alias = {});
    void AddFrom(const std::string & clause, const std::string & alias = {});
    void AddWhereString(const std::string & operand1, const std::string & operatr, const std::string & operand2)
    {
        whereClause.push_back(ConvertName(operand1) + operatr + QuoteString(operand2));
    }
    void AddWhereContains(const std::string & column, const std::string & subString);
    void AddWhere(const std::string & operand1, const std::string & operatr, const std::string & operand2)
    {
        whereClause.push_back(ConvertName(operand1) + operatr + operand2);
    }
    template <class T> void AddWhere(const std::string & operand1, const std::string & operatr, T operand2)
    {
        whereClause.push_back(ConvertName(operand1) + operatr + Core::Serialize(operand2));
    }
    void AddOrderBy(const std::string & clause)
    {
        orderClause.push_back(clause);
    }
    bool GetSelectDistinct() { return distinct; }
    void SetSelectDistinct(bool value) { distinct = value; }

    virtual std::string ToString() const;

    std::string SelectClause() const;
    std::string FromClause() const;
    std::string WhereClause() const;
    std::string OrderClause() const;

private:
    std::vector< std::string > selectClause;
    std::vector< std::string > fromClause;
    std::vector< std::string > whereClause;
    std::vector< std::string > orderClause;
    bool distinct;
};

} // namespace Database
