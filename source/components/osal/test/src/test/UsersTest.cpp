#include <unittest-cpp/UnitTestC++.h>

#include "osal/Users.h"

using namespace std;

namespace OSAL {
namespace Users {
namespace Test {

class UserIDTest : public UnitTestCpp::TestFixture
{
public:
};

class GroupIDTest : public UnitTestCpp::TestFixture
{
public:
};

TEST_SUITE(osal)
{

TEST_FIXTURE(UserIDTest, Constructor)
{
    OSAL::Users::UserID uid;
    EXPECT_EQ(__uid_t { 0xFFFFFFFF }, uid);
    EXPECT_EQ(UserID::Invalid, uid);
    EXPECT_EQ("-", uid.UserName());
}

TEST_FIXTURE(UserIDTest, ConstructUID)
{
    OSAL::Users::UserID uid(0); // root
    EXPECT_EQ(__uid_t { 0 }, uid);
    EXPECT_NE(UserID::Invalid, uid);
    EXPECT_EQ("root", uid.UserName());
}

TEST_FIXTURE(UserIDTest, GetUserID)
{
        OSAL::Users::UserID uid = OSAL::Users::UserID::GetUserID();
    EXPECT_NE(__uid_t { 0xFFFFFFFF }, uid);
    EXPECT_NE(UserID::Invalid, uid);
    EXPECT_NE("-", uid.UserName());

}

TEST_FIXTURE(GroupIDTest, Constructor)
{
    OSAL::Users::GroupID gid;
    EXPECT_EQ(__gid_t { 0xFFFFFFFF }, gid);
    EXPECT_EQ(GroupID::Invalid, gid);
    EXPECT_EQ("-", gid.GroupName());
}

TEST_FIXTURE(GroupIDTest, ConstructorGID)
{
    OSAL::Users::GroupID gid(0);
    EXPECT_EQ(__gid_t { 0 }, gid);
    EXPECT_NE(GroupID::Invalid, gid);
    EXPECT_EQ("root", gid.GroupName());
}

TEST_FIXTURE(UserIDTest, GetGroupID)
{
    OSAL::Users::GroupID gid = OSAL::Users::GroupID::GetGroupID();
    EXPECT_NE(__gid_t { 0xFFFFFFFF }, gid);
    EXPECT_NE(GroupID::Invalid, gid);
    EXPECT_NE("-", gid.GroupName());
}

} // TEST_SUITE(osal)

} // namespace Test
} // namespace Users
} // namespace OSAL
