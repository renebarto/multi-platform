#pragma once

#include <vector>
#include "core/serialization/SerializationImpl.h"
#include "database/CommandQuery.h"

namespace Database
{

class DATABASE_EXPORT InsertQuery: public CommandQuery
{
public:
    typedef std::shared_ptr<InsertQuery> Ptr;

    InsertQuery();
    virtual ~InsertQuery();

    void AddInsertInto(const std::string & tableName)
    {
        insertClause.push_back(tableName);
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

    void AddOnDuplicateSetString(const std::string & operand1, const std::string & operand2)
    {
        onDuplicateSetClause.push_back(ConvertName(operand1) + "=" + QuoteString(operand2));
    }

    void AddOnDuplicateSet(const std::string & operand1, const std::string & operand2)
    {
        onDuplicateSetClause.push_back(ConvertName(operand1) + "=" + operand2);
    }

    template <class T> void AddOnDuplicateSet(const std::string & operand1, T operand2)
    {
        onDuplicateSetClause.push_back(ConvertName(operand1) + "=" + Core::Serialize(operand2));
    }

    virtual std::string ToString() const;

private:
    std::vector< std::string > insertClause;
    std::vector< std::string > setClause;
    std::vector< std::string > onDuplicateSetClause;
};

} // namespace Database
