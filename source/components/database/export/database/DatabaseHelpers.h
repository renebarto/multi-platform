#pragma once

#include <string>
#include "database/exports.h"

namespace Database
{

class DATABASE_EXPORT DatabaseHelpers
{
public:
    static void MakeSureSchemaDoesNotExist(const std::string & schema, const std::string & location,
                                           const std::string & userID, const std::string & password);
    static bool SchemaExists(const std::string & schema, const std::string & location,
                             const std::string & userID, const std::string & password);
    static bool CompareSQLFiles(const char* pathA, const char* pathB);
    static bool CompareSQLFiles(const std::string & pathA, const std::string & pathB);

protected:
private:
};

} // namespace Database
