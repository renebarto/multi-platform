#include "database/DatabaseHelpers.h"

#include <cstring>
#include "database/DBEngine.h"
#include "database/IDatabase.h"

using namespace std;
using namespace Database;

void DatabaseHelpers::MakeSureSchemaDoesNotExist(const string & schema, const string & location,
                                                  const string & userID, const string & password)
{
    DBEngine::Ptr engine(new DBEngine());

    try
    {
        DBEngine::DeleteDatabase(engine, schema, location, userID, password);
    }
    catch (...)
    {
    }
}

bool DatabaseHelpers::SchemaExists(const string & schema, const string & location,
                                    const string & userID, const string & password)
{
    bool exists;
    DBEngine::Ptr engine(new DBEngine());

    IDatabase::Ptr database = DBEngine::OpenDatabase(engine, schema, location, userID, password);
    exists = (database != 0);

    return exists;
}

class TextFile
{
public:
    TextFile() :
        handle(0)
    {
    }
    TextFile(const TextFile &) = delete;
    ~TextFile() { Close(); }
    TextFile & operator = (const TextFile &) = delete;

    operator FILE * () { return handle;  }
    bool OpenRead(const char * path)
    {
        Close();
        handle = fopen(path, "r");
        return (handle != 0);
    }
    bool OpenWrite(const char * path)
    {
        Close();
        handle = fopen(path, "w");
        return (handle != 0);
    }
    void Close()
    {
        if (handle != 0)
            fclose(handle);
        handle = 0;
    }

private:
    FILE * handle;
};

class MallocBuffer
{
public:
    MallocBuffer() = delete;
    MallocBuffer(const MallocBuffer &) = delete;
    MallocBuffer(size_t size) :
        size(size),
        contents(0)
    {
        contents = (char *)malloc(size);
    }
    ~MallocBuffer()
    {
        free(contents);
        size = 0;
        contents = 0;
    }

    MallocBuffer & operator = (const MallocBuffer &) = delete;

    size_t size;
    char * contents;
};

ssize_t ReadNonCommentLine(MallocBuffer & buffer, FILE * file)
{
    ssize_t count = 0;
    do
    {
        count = getline(&buffer.contents, &buffer.size, file);
    }
    while ((count >= 2) && (strstr(buffer.contents, "--") == buffer.contents));
    return count;
}

bool CompareLines(MallocBuffer & bufferA, TextFile & fileA, MallocBuffer & bufferB, TextFile & fileB)
{
    while (!feof(fileA) && !feof(fileB))
    {
        ssize_t countA = ReadNonCommentLine(bufferA, fileA);
        ssize_t countB = ReadNonCommentLine(bufferB, fileB);
        if ((countA == -1) || (countB == -1))
            break;
        if (countA != countB)
        {
            return false;
        }
        if (strcmp(bufferA.contents, bufferB.contents) != 0)
            return false;
    }
    return (feof(fileA) && feof(fileB));
}

bool DatabaseHelpers::CompareSQLFiles(const char* pathA, const char* pathB)
{
    bool result = false;
    TextFile fileA;
    TextFile fileB;

    if (fileA.OpenRead(pathA))
    {
        if (fileB.OpenRead(pathB))
        {
            const size_t BufferSize = 1024;
            MallocBuffer bufferA(BufferSize);
            MallocBuffer bufferB(BufferSize);
            result = CompareLines(bufferA, fileA, bufferB, fileB);
        }
    }
    return result;
}

bool DatabaseHelpers::CompareSQLFiles(const string & pathA, const string & pathB)
{
    return CompareSQLFiles(pathA.c_str(), pathB.c_str());
}
