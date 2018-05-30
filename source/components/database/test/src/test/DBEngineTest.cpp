#include <unittest-cpp/UnitTestC++.h>
#include "osal/Strings.h"
#include "database/TestData.h"
#include "database/DBEngine.h"
#include "database/DatabaseHelpers.h"
#include "database/IDatabase.h"

namespace Database
{

namespace Test
{

static const string TableName = "table1";
static const string QueryText = "SELECT * FROM `table1`";
static const string CommandText = "DROP TABLE `table1`";

class DBEngineTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    DBEngine::Ptr engine;
};

void DBEngineTest::SetUp()
{
    engine = make_shared<DBEngine>();
    DBEngine::SetDefaultLocation(Database::Test::Data::Location());
    DBEngine::SetDefaultUserID(Database::Test::Data::Userid());
    DBEngine::SetDefaultPassword(Database::Test::Data::Password());
    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseSimple(),
                                                 Database::Test::Data::Location(),
                                                 Database::Test::Data::Userid(),
                                                 Database::Test::Data::Password());
    DBEngine::RestoreDatabase(engine,
                              Database::Test::Data::DatabaseImportSimpleRef(),
                              Database::Test::Data::DatabaseSimple(),
                              Database::Test::Data::Location(),
                              Database::Test::Data::Userid(),
                              Database::Test::Data::Password());
    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseTmp(),
                                                 Database::Test::Data::Location(),
                                                 Database::Test::Data::Userid(),
                                                 Database::Test::Data::Password());
}

void DBEngineTest::TearDown()
{
    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseSimple(),
                                                 Database::Test::Data::Location(),
                                                 Database::Test::Data::Userid(),
                                                 Database::Test::Data::Password());
    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseTmp(),
                                                 Database::Test::Data::Location(),
                                                 Database::Test::Data::Userid(),
                                                 Database::Test::Data::Password());
}

TEST_FIXTURE(DBEngineTest, Construction)
{
    EXPECT_EQ(engine->GetDefaultLocation(), Database::Test::Data::Location());
    EXPECT_EQ(engine->GetDefaultUserID(), Database::Test::Data::Userid());
    EXPECT_EQ(engine->GetDefaultPassword(), Database::Test::Data::Password());
}

TEST_FIXTURE(DBEngineTest, OpenDatabaseNonExistent)
{
    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseDummy(),
                                                 Database::Test::Data::Location(),
                                                 Database::Test::Data::Userid(),
                                                 Database::Test::Data::Password());
    IDatabasePtr database = DBEngine::OpenDatabase(engine, Database::Test::Data::DatabaseDummy());
    EXPECT_TRUE(database == nullptr);
}

TEST_FIXTURE(DBEngineTest, OpenDatabaseExistent)
{
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                              Database::Test::Data::Location(),
                                              Database::Test::Data::Userid(),
                                              Database::Test::Data::Password()));
    IDatabasePtr database = DBEngine::OpenDatabase(engine, Database::Test::Data::DatabaseSimple());

    EXPECT_FALSE(database == nullptr);
    EXPECT_TRUE(database->IsOpen());
    database->Close();
    EXPECT_FALSE(database->IsOpen());

    database.reset();
}

TEST_FIXTURE(DBEngineTest, OpenDatabase1)
{
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    IDatabasePtr database = DBEngine::OpenDatabase(engine, Database::Test::Data::DatabaseSimple(),
                                                   Database::Test::Data::Location());

    EXPECT_FALSE(database == nullptr);
    EXPECT_TRUE(database->IsOpen());
    database->Close();
    EXPECT_FALSE(database->IsOpen());

    database.reset();
}

TEST_FIXTURE(DBEngineTest, OpenDatabase2)
{
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    IDatabasePtr database = DBEngine::OpenDatabase(engine,
                                                   Database::Test::Data::DatabaseSimple(),
                                                   Database::Test::Data::Location(),
                                                   Database::Test::Data::Userid(),
                                                   Database::Test::Data::Password());

    EXPECT_FALSE(database == nullptr);
    EXPECT_TRUE(database->IsOpen());
    database->Close();
    EXPECT_FALSE(database->IsOpen());

    database.reset();
}

TEST_FIXTURE(DBEngineTest, CreateDatabase)
{
    bool success = DBEngine::CreateDatabase(engine, Database::Test::Data::DatabaseTmp());
    EXPECT_TRUE(success);
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseTmp(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
}

TEST_FIXTURE(DBEngineTest, CreateDatabase1)
{
    bool success = DBEngine::CreateDatabase(engine, Database::Test::Data::DatabaseTmp(),
                                            Database::Test::Data::Location());
    EXPECT_TRUE(success);
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseTmp(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
}

TEST_FIXTURE(DBEngineTest, CreateDatabase2)
{
    bool success = DBEngine::CreateDatabase(engine, Database::Test::Data::DatabaseTmp(),
                                            Database::Test::Data::Location(),
                                            Database::Test::Data::Userid(),
                                            Database::Test::Data::Password());
    EXPECT_TRUE(success);
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseTmp(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
}

TEST_FIXTURE(DBEngineTest, CreateAndOpenDatabase)
{
    IDatabasePtr database = DBEngine::CreateAndOpenDatabase(engine, Database::Test::Data::DatabaseTmp());
    EXPECT_FALSE(database == nullptr);
    EXPECT_TRUE(database->IsOpen());
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseTmp(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    database->Close();
    EXPECT_FALSE(database->IsOpen());
    database.reset();
}

TEST_FIXTURE(DBEngineTest, CreateAndOpenDatabase1)
{
    IDatabasePtr database = DBEngine::CreateAndOpenDatabase(engine, Database::Test::Data::DatabaseTmp(),
                                                            Database::Test::Data::Location());
    EXPECT_FALSE(database == nullptr);
    EXPECT_TRUE(database->IsOpen());
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseTmp(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    database->Close();
    EXPECT_FALSE(database->IsOpen());
    database.reset();
}

TEST_FIXTURE(DBEngineTest, CreateAndOpenDatabase2)
{
    IDatabasePtr database = DBEngine::CreateAndOpenDatabase(engine, Database::Test::Data::DatabaseTmp(),
                                                            Database::Test::Data::Location(),
                                                            Database::Test::Data::Userid(),
                                                            Database::Test::Data::Password());
    EXPECT_FALSE(database == nullptr);
    EXPECT_TRUE(database->IsOpen());
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseTmp(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    database->Close();
    EXPECT_FALSE(database->IsOpen());
    database.reset();
}

TEST_FIXTURE(DBEngineTest, DeleteDatabaseOpen)
{
    IDatabasePtr database = DBEngine::OpenDatabase(engine, Database::Test::Data::DatabaseSimple());
    EXPECT_TRUE(database->IsOpen());
    EXPECT_TRUE(DBEngine::DeleteDatabase(engine, Database::Test::Data::DatabaseSimple()));
    database->Close();
    EXPECT_FALSE(database->IsOpen());
    database.reset();
}

TEST_FIXTURE(DBEngineTest, DeleteDatabaseClosed)
{
    IDatabasePtr database = DBEngine::OpenDatabase(engine, Database::Test::Data::DatabaseSimple());
    EXPECT_TRUE(database->IsOpen());
    database->Close();
    EXPECT_FALSE(database->IsOpen());
    EXPECT_TRUE(DBEngine::DeleteDatabase(engine, Database::Test::Data::DatabaseSimple()));
    database.reset();
}

TEST_FIXTURE(DBEngineTest, DeleteDatabase1)
{
    IDatabasePtr database = DBEngine::OpenDatabase(engine, Database::Test::Data::DatabaseSimple());
    EXPECT_TRUE(database->IsOpen());
    database->Close();
    EXPECT_FALSE(database->IsOpen());
    EXPECT_TRUE(DBEngine::DeleteDatabase(engine, Database::Test::Data::DatabaseSimple(),
                                         Database::Test::Data::Location()));
    database.reset();
}

TEST_FIXTURE(DBEngineTest, DeleteDatabase2)
{
    IDatabasePtr database = DBEngine::OpenDatabase(engine, Database::Test::Data::DatabaseSimple());
    EXPECT_TRUE(database->IsOpen());
    database->Close();
    EXPECT_FALSE(database->IsOpen());
    EXPECT_TRUE(DBEngine::DeleteDatabase(engine, Database::Test::Data::DatabaseSimple(),
                                         Database::Test::Data::Location(),
                                         Database::Test::Data::Userid(),
                                         Database::Test::Data::Password()));
    database.reset();
}

TEST_FIXTURE(DBEngineTest, GetDatabasesDefinitions)
{
    DataTable defs = DBEngine::GetDatabasesDefinitions(engine, Database::Test::Data::DatabaseSimple());
    EXPECT_FALSE(defs.num_rows() == 0);

    vector<string> schemas;
    for (auto row : defs)
    {
        schemas.push_back((string)row["SCHEMA_NAME"]);
    }
    bool foundSchema = false;
    for (auto schemaName : schemas)
    {
        if (OSAL::Strings::IsEqualIgnoreCase(schemaName, Database::Test::Data::DatabaseSimple()))
        {
            foundSchema = true;
            break;
        }
    }

    EXPECT_TRUE(foundSchema);
}

TEST_FIXTURE(DBEngineTest, GetDatabasesDefinitions1)
{
    DataTable defs = DBEngine::GetDatabasesDefinitions(engine,
                                                       Database::Test::Data::DatabaseSimple(),
                                                       Database::Test::Data::Location());
    EXPECT_FALSE(defs.num_rows() == 0);

    vector<string> schemas;
    for (auto row : defs)
    {
        schemas.push_back((string)row["SCHEMA_NAME"]);
    }
    bool foundSchema = false;
    for (auto schemaName : schemas)
    {
        if (OSAL::Strings::IsEqualIgnoreCase(schemaName, Database::Test::Data::DatabaseSimple()))
        {
            foundSchema = true;
            break;
        }
    }

    EXPECT_TRUE(foundSchema);
}

TEST_FIXTURE(DBEngineTest, GetDatabasesDefinitions2)
{
    DataTable defs = DBEngine::GetDatabasesDefinitions(engine,
                                                       Database::Test::Data::DatabaseSimple(),
                                                       Database::Test::Data::Location(),
                                                       Database::Test::Data::Userid(),
                                                       Database::Test::Data::Password());
    EXPECT_FALSE(defs.num_rows() == 0);

    vector<string> schemas;
    for (auto row : defs)
    {
        schemas.push_back((string)row["SCHEMA_NAME"]);
    }
    bool foundSchema = false;
    for (auto schemaName : schemas)
    {
        if (OSAL::Strings::IsEqualIgnoreCase(schemaName, Database::Test::Data::DatabaseSimple()))
        {
            foundSchema = true;
            break;
        }
    }

    EXPECT_TRUE(foundSchema);
}

TEST_FIXTURE(DBEngineTest, RestoreDatabase)
{
    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseSimple(),
                                                 Database::Test::Data::Location(),
                                                 Database::Test::Data::Userid(),
                                                 Database::Test::Data::Password());
    EXPECT_TRUE(DBEngine::RestoreDatabase(engine, Database::Test::Data::DatabaseImportSimpleRef(), Database::Test::Data::DatabaseSimple()));
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(), Database::Test::Data::Location(), Database::Test::Data::Userid(), Database::Test::Data::Password()));
}

TEST_FIXTURE(DBEngineTest, RestoreDatabase1)
{
    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseSimple(), Database::Test::Data::Location(), Database::Test::Data::Userid(), Database::Test::Data::Password());
    EXPECT_TRUE(DBEngine::RestoreDatabase(engine, Database::Test::Data::DatabaseImportSimpleRef(),
                                          Database::Test::Data::DatabaseSimple(),
                                          Database::Test::Data::Location()));
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
}

TEST_FIXTURE(DBEngineTest, RestoreDatabase2)
{
    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseSimple(),
                                                 Database::Test::Data::Location(),
                                                 Database::Test::Data::Userid(),
                                                 Database::Test::Data::Password());
    EXPECT_TRUE(DBEngine::RestoreDatabase(engine, Database::Test::Data::DatabaseImportSimpleRef(),
                                          Database::Test::Data::DatabaseSimple(),
                                          Database::Test::Data::Location(),
                                          Database::Test::Data::Userid(),
                                          Database::Test::Data::Password()));
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
}

//TEST_FIXTURE(DBEngineTest, BackupDatabase)
//{
//    EXPECT_TRUE(DBEngine::BackupDatabase(engine,
//                                         Database::Test::Data::DatabaseImportSimple2(),
//                                         Database::Test::Data::DatabaseSimple()));
//    EXPECT_TRUE(DatabaseHelpers::CompareSQLFiles(Database::Test::Data::DatabaseImportSimple2Ref(),
//                                                  Database::Test::Data::DatabaseImportSimple2()));
//    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseSimple(),
//                                                 Database::Test::Data::Location(),
//                                                 Database::Test::Data::Userid(),
//                                                 Database::Test::Data::Password());
//    EXPECT_FALSE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
//                                                Database::Test::Data::Location(),
//                                                Database::Test::Data::Userid(),
//                                                Database::Test::Data::Password()));
//    EXPECT_TRUE(DBEngine::RestoreDatabase(engine, Database::Test::Data::DatabaseImportSimple2(),
//                                          Database::Test::Data::DatabaseSimple()));
//    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
//                                               Database::Test::Data::Location(),
//                                               Database::Test::Data::Userid(),
//                                               Database::Test::Data::Password()));
//    OSAL::Path::MakeSureFileDoesNotExist(Database::Test::Data::DatabaseImportSimple2());
//}
//
//TEST_FIXTURE(DBEngineTest, BackupDatabase1)
//{
//    EXPECT_TRUE(DBEngine::BackupDatabase(engine,
//                                         Database::Test::Data::DatabaseImportSimple2(),
//                                         Database::Test::Data::DatabaseSimple(),
//                                         Database::Test::Data::Location()));
//    EXPECT_TRUE(DatabaseHelpers::CompareSQLFiles(Database::Test::Data::DatabaseImportSimple2Ref(),
//                                                  Database::Test::Data::DatabaseImportSimple2()));
//    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseSimple(),
//                                                 Database::Test::Data::Location(),
//                                                 Database::Test::Data::Userid(),
//                                                 Database::Test::Data::Password());
//    EXPECT_FALSE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
//                                                Database::Test::Data::Location(),
//                                                Database::Test::Data::Userid(),
//                                                Database::Test::Data::Password()));
//    EXPECT_TRUE(DBEngine::RestoreDatabase(engine, Database::Test::Data::DatabaseImportSimple2(),
//                                          Database::Test::Data::DatabaseSimple()));
//    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
//                                               Database::Test::Data::Location(),
//                                               Database::Test::Data::Userid(),
//                                               Database::Test::Data::Password()));
//    OSAL::Path::MakeSureFileDoesNotExist(Database::Test::Data::DatabaseImportSimple2());
//}
//
//TEST_FIXTURE(DBEngineTest, BackupDatabase2)
//{
//    EXPECT_TRUE(DBEngine::BackupDatabase(engine,
//                                         Database::Test::Data::DatabaseImportSimple2(),
//                                         Database::Test::Data::DatabaseSimple(),
//                                         Database::Test::Data::Location(),
//                                         Database::Test::Data::Userid(),
//                                         Database::Test::Data::Password()));
//    EXPECT_TRUE(DatabaseHelpers::CompareSQLFiles(Database::Test::Data::DatabaseImportSimple2Ref(),
//                                                  Database::Test::Data::DatabaseImportSimple2()));
//    DatabaseHelpers::MakeSureSchemaDoesNotExist(Database::Test::Data::DatabaseSimple(),
//                                                 Database::Test::Data::Location(),
//                                                 Database::Test::Data::Userid(),
//                                                 Database::Test::Data::Password());
//    EXPECT_FALSE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
//                                                Database::Test::Data::Location(),
//                                                Database::Test::Data::Userid(),
//                                                Database::Test::Data::Password()));
//    EXPECT_TRUE(DBEngine::RestoreDatabase(engine, Database::Test::Data::DatabaseImportSimple2(),
//                                          Database::Test::Data::DatabaseSimple()));
//    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
//                                               Database::Test::Data::Location(),
//                                               Database::Test::Data::Userid(),
//                                               Database::Test::Data::Password()));
//    OSAL::Path::MakeSureFileDoesNotExist(Database::Test::Data::DatabaseImportSimple2());
//}

TEST_FIXTURE(DBEngineTest, RestoreTable)
{
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    EXPECT_TRUE(DBEngine::RestoreTable(engine,
                                       Database::Test::Data::DatabaseImportSimple2TableRef(),
                                       TableName,
                                       Database::Test::Data::DatabaseSimple()));
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
}

TEST_FIXTURE(DBEngineTest, RestoreTable1)
{
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    EXPECT_TRUE(DBEngine::RestoreTable(engine, Database::Test::Data::DatabaseImportSimple2TableRef(),
                                     TableName,
                                     Database::Test::Data::DatabaseSimple(),
                                     Database::Test::Data::Location()));
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
}

TEST_FIXTURE(DBEngineTest, RestoreTable2)
{
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    EXPECT_TRUE(DBEngine::RestoreTable(engine, Database::Test::Data::DatabaseImportSimple2TableRef(),
                                     TableName,
                                     Database::Test::Data::DatabaseSimple(),
                                     Database::Test::Data::Location(),
                                     Database::Test::Data::Userid(),
                                     Database::Test::Data::Password()));
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
}

TEST_FIXTURE(DBEngineTest, BackupTable)
{
    EXPECT_TRUE(DBEngine::BackupTable(engine,
                                      Database::Test::Data::DatabaseImportSimple2(),
                                      TableName,
                                      Database::Test::Data::DatabaseSimple()));
    EXPECT_TRUE(DatabaseHelpers::CompareSQLFiles(Database::Test::Data::DatabaseImportSimple2TableRef(),
                                                  Database::Test::Data::DatabaseImportSimple2()));
    EXPECT_TRUE(DBEngine::RestoreTable(engine, Database::Test::Data::DatabaseImportSimple2(),
                                     TableName, Database::Test::Data::DatabaseSimple()));
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    OSAL::Path::MakeSureFileDoesNotExist(Database::Test::Data::DatabaseImportSimple2());
}

TEST_FIXTURE(DBEngineTest, BackupTable1)
{
    EXPECT_TRUE(DBEngine::BackupTable(engine,
                                      Database::Test::Data::DatabaseImportSimple2(),
                                      TableName,
                                      Database::Test::Data::DatabaseSimple(),
                                      Database::Test::Data::Location()));
    EXPECT_TRUE(DatabaseHelpers::CompareSQLFiles(Database::Test::Data::DatabaseImportSimple2TableRef(),
                                                  Database::Test::Data::DatabaseImportSimple2()));
    EXPECT_TRUE(DBEngine::RestoreTable(engine,
                                       Database::Test::Data::DatabaseImportSimple2(),
                                       TableName,
                                       Database::Test::Data::DatabaseSimple()));
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    OSAL::Path::MakeSureFileDoesNotExist(Database::Test::Data::DatabaseImportSimple2());
}

TEST_FIXTURE(DBEngineTest, BackupTable2)
{
    EXPECT_TRUE(DBEngine::BackupTable(engine,
                                      Database::Test::Data::DatabaseImportSimple2(),
                                      TableName,
                                      Database::Test::Data::DatabaseSimple(),
                                      Database::Test::Data::Location(),
                                      Database::Test::Data::Userid(),
                                      Database::Test::Data::Password()));
    EXPECT_TRUE(DatabaseHelpers::CompareSQLFiles(Database::Test::Data::DatabaseImportSimple2TableRef(),
                                                  Database::Test::Data::DatabaseImportSimple2()));
    EXPECT_TRUE(DBEngine::RestoreTable(engine,
                                       Database::Test::Data::DatabaseImportSimple2(),
                                       TableName,
                                       Database::Test::Data::DatabaseSimple()));
    EXPECT_TRUE(DatabaseHelpers::SchemaExists(Database::Test::Data::DatabaseSimple(),
                                               Database::Test::Data::Location(),
                                               Database::Test::Data::Userid(),
                                               Database::Test::Data::Password()));
    OSAL::Path::MakeSureFileDoesNotExist(Database::Test::Data::DatabaseImportSimple2());
}

TEST_FIXTURE(DBEngineTest, ExecuteQuery)
{
    IDatabasePtr database = DBEngine::OpenDatabase(engine, Database::Test::Data::DatabaseSimple());
    ConnectionPtr connection = database->GetConnection();
    DataReader reader = DBEngine::ExecuteQuery(connection, QueryText);
    DataRow row = reader.NextRow();
    ASSERT_FALSE(!row);
    row = reader.NextRow();
    ASSERT_FALSE(!row);
    row = reader.NextRow();
    ASSERT_TRUE(!row);
}

TEST_FIXTURE(DBEngineTest, ExecuteCommand)
{
    IDatabasePtr database = DBEngine::OpenDatabase(engine, Database::Test::Data::DatabaseSimple());
    ConnectionPtr connection = database->GetConnection();
    DataTable tableDefs = database->GetTablesDefinitions();
    EXPECT_EQ(size_t(2), tableDefs.num_rows());
    DBEngine::ExecuteCommand(connection, CommandText);
    tableDefs = database->GetTablesDefinitions();
    EXPECT_EQ(size_t(1), tableDefs.num_rows());
}

} // namespace Test

} // namespace DBSupport

