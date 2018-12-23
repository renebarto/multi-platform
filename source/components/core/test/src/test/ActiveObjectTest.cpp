#include <unittest-cpp/UnitTestC++.h>

#include <iostream>
#include "osal/Time.h"
#include "core/ActiveObject.h"

namespace Core {
namespace Test {

TEST_SUITE(core) {

static auto SLEEP = std::chrono::milliseconds(100);

class MyActiveObject: public ActiveObject<int>
{
public:
    static bool isDeleted;

    MyActiveObject(): ActiveObject("ActiveObjTst1")
    {
        MyActiveObject::isDeleted = false;
    }
    ~MyActiveObject()
    {
        isDeleted = true;
    }
    virtual void InitThread() {}
    virtual int Run() { OSAL::Time::Sleep(SLEEP); return 1; }
    virtual void ExitThread() {}
    virtual void FlushThread() {}
};

bool MyActiveObject::isDeleted;

class MyActiveObject2: public ActiveObject<int>
{
public:
    MyActiveObject2() : ActiveObject("ActiveObjTst2") {}
    virtual void InitThread() { }
    virtual int Run()
    {
        while (!IsDying())
        {
            OSAL::Time::Sleep(SLEEP * 2);
        }
        return 2;
    }
    virtual void ExitThread() { }
    virtual void FlushThread() { }
};

class ActiveObjectTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void ActiveObjectTest::SetUp()
{
}

void ActiveObjectTest::TearDown()
{
}

TEST_FIXTURE(ActiveObjectTest, Simple)
{
    MyActiveObject object;
    object.Create();
    OSAL::Time::Sleep(SLEEP);
    object.Kill();
    EXPECT_EQ(1, object.GetResult());
}

TEST_FIXTURE(ActiveObjectTest, Loop)
{
    MyActiveObject2 object;
    object.Create();
    OSAL::Time::Sleep(SLEEP);
    object.Kill();
    EXPECT_EQ(2, object.GetResult());
}

} // TEST_SUITE(core)

} // namespace Test
} // namespace Core
