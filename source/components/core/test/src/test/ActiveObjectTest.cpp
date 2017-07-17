#include <unit-test-c++/UnitTestC++.h>

#include <iostream>
#include "core/ActiveObject.h"
#include "core/Util.h"

namespace Core
{

namespace Test
{

static const int SLEEP = 100;

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
    virtual int Run() { Core::Util::Sleep(SLEEP); return 1; }
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
            Core::Util::Sleep(SLEEP * 2);
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

TEST_SUITE(core)
{

TEST_FIXTURE(ActiveObjectTest, Simple)
{
    MyActiveObject object;
    object.Create();
    Core::Util::Sleep(SLEEP);
    object.Kill();
    EXPECT_EQ(1, object.GetResult());
}

TEST_FIXTURE(ActiveObjectTest, Loop)
{
    MyActiveObject2 object;
    object.Create();
    Core::Util::Sleep(SLEEP);
    object.Kill();
    EXPECT_EQ(2, object.GetResult());
}

} // TEST_SUITE(core)

} // namespace Test

} // namespace Core
