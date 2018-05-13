#include "osal/Users.h"

#include <pwd.h>
#include <grp.h>

using namespace OSAL;
using namespace Users;

static const uid_t InvalidUserID = 0xFFFFFFFF;
static const gid_t InvalidGroupID = 0xFFFFFFFF;
const UserID UserID::Invalid = UserID(InvalidUserID);
const GroupID GroupID::Invalid = GroupID(InvalidGroupID);

UserID::UserID()
    : _uid(InvalidUserID)
{}

std::string UserID::UserName()
{
    struct passwd * passwdInfo = getpwuid(_uid);
    if (passwdInfo)
        return passwdInfo->pw_name;
    return "-";
}

GroupID::GroupID()
    : _gid(InvalidGroupID)
{}

std::string GroupID::GroupName()
{
    struct group * groupInfo = getgrgid(_gid);
    if (groupInfo)
        return groupInfo->gr_name;
    return "-";
}

