#pragma once

#include <vector>
#include "database/ColumnDefinition.h"
#include "database/CommandQuery.h"

namespace Database
{

class DATABASE_EXPORT CreateTableQuery: public CommandQuery
{
public:
    typedef std::shared_ptr<CreateTableQuery> Ptr;

    static const std::string CreateTableString;
    static const std::string NotNullString;
    static const std::string UniqueKeyString;
    static const std::string AutoIncrementString;

    explicit CreateTableQuery(const std::string & tableName);
    virtual ~CreateTableQuery();

    const std::string & GetTableName() const;

    void AddColumn(const std::string & columnName, const std::type_info & columnType);
    void AddColumn(const std::string & columnName, const std::type_info & columnType, int size);
    void AddColumn(const std::string & columnName, const std::type_info & columnType, int size, bool nullable);
    void AddColumn(const std::string & columnName, const std::type_info & columnType, int size, bool nullable, bool autoIncrement);
    void AddColumn(const std::string & columnName, const std::type_info & columnType, int size, bool nullable, bool autoIncrement, bool uniqueKey);
    void AddColumn(const std::string & columnName, const std::string & columnType);

    virtual std::string ToString() const;

private:
    std::string tableName;
    std::vector<ColumnDefinition> columns;
};

} // namespace Database
