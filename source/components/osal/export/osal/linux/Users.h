#pragma once

#include <sys/types.h>
#include <unistd.h>

namespace OSAL {
namespace Users {

class UserID
{
public:
    UserID()
        : _uid()
    {}
    UserID(const UserID &) = delete;
    UserID(UserID && other)
    {
        _uid = other._uid;
    }
    explicit UserID(__uid_t uid)
        : _uid(uid)
    {}
    UserID & operator = (const UserID &) = delete;
    UserID & operator = (__uid_t uid)
    {
        _uid = uid;
        return *this;
    }
    bool Equals(const UserID & other) const
    {
        return _uid == other._uid;
    }

    static UserID GetUserID()
    {
        return UserID(getuid());
    }

private:
    __uid_t _uid;
};

bool operator == (const UserID & lhs, const UserID & rhs)
{
    return lhs.Equals(rhs);
}
bool operator != (const UserID & lhs, const UserID & rhs)
{
    return !lhs.Equals(rhs);
}

class GroupID
{
public:
    GroupID()
        : _gid()
    {}
    GroupID(const GroupID &) = delete;
    GroupID(GroupID && other)
    {
        _gid = other._gid;
    }
    explicit GroupID(__gid_t gid)
        : _gid(gid)
    {}
    GroupID & operator = (const GroupID &) = delete;
    GroupID & operator = (__gid_t gid)
    {
        _gid = gid;
        return *this;
    }
    bool Equals(const GroupID & other) const
    {
        return _gid == other._gid;
    }

    static GroupID GetGroupID()
    {
        return GroupID(getgid());
    }

private:
    __gid_t _gid;
};

bool operator == (const GroupID & lhs, const GroupID & rhs)
{
    return lhs.Equals(rhs);
}
bool operator != (const GroupID & lhs, const GroupID & rhs)
{
    return !lhs.Equals(rhs);
}

} // namespace Users
} // namespace OSAL
