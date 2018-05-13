#pragma once

#include <string>
#include <sys/types.h>
#include <unistd.h>

namespace OSAL {
namespace Users {

class UserID
{
public:
    static const UserID Invalid;
    UserID();
    UserID(const UserID & other)
    {
        _uid = other._uid;
    }
    UserID(UserID && other)
    {
        _uid = other._uid;
    }
    explicit UserID(__uid_t uid)
        : _uid(uid)
    {}
    UserID & operator = (const UserID & other)
    {
        if (&other != this)
        {
            _uid = other._uid;
        }
        return *this;
    }
    UserID & operator = (UserID && other)
    {
        if (&other != this)
        {
            _uid = other._uid;
        }
        return *this;
    }
    UserID & operator = (__uid_t uid)
    {
        _uid = uid;
        return *this;
    }
    bool Equals(const UserID & other) const
    {
        return _uid == other._uid;
    }
    bool Equals(const __uid_t & other) const
    {
        return _uid == other;
    }

    static UserID GetUserID()
    {
        return UserID(getuid());
    }

    std::string UserName();

private:
    __uid_t _uid;
};

inline bool operator == (const UserID & lhs, const UserID & rhs)
{
    return lhs.Equals(rhs);
}
inline bool operator != (const UserID & lhs, const UserID & rhs)
{
    return !lhs.Equals(rhs);
}
inline bool operator == (const __uid_t & lhs, const UserID & rhs)
{
    return rhs.Equals(lhs);
}
inline bool operator != (const __uid_t & lhs, const UserID & rhs)
{
    return !rhs.Equals(lhs);
}
inline bool operator == (const UserID & lhs, const __uid_t & rhs)
{
    return lhs.Equals(rhs);
}
inline bool operator != (const UserID & lhs, const __uid_t & rhs)
{
    return !lhs.Equals(rhs);
}

class GroupID
{
public:
    static const GroupID Invalid;
    GroupID();
    GroupID(const GroupID & other)
    {
        _gid = other._gid;
    }
    GroupID(GroupID && other)
    {
        _gid = other._gid;
    }
    explicit GroupID(__gid_t gid)
        : _gid(gid)
    {}
    GroupID & operator = (const GroupID & other)
    {
        if (&other != this)
        {
            _gid = other._gid;
        }
        return *this;
    }
    GroupID & operator = (GroupID && other)
    {
        if (&other != this)
        {
            _gid = other._gid;
        }
        return *this;
    }
    GroupID & operator = (__gid_t gid)
    {
        _gid = gid;
        return *this;
    }
    bool Equals(const GroupID & other) const
    {
        return _gid == other._gid;
    }
    bool Equals(const __gid_t & other) const
    {
        return _gid == other;
    }

    static GroupID GetGroupID()
    {
        return GroupID(getgid());
    }

    std::string GroupName();

private:
    __gid_t _gid;
};

inline bool operator == (const GroupID & lhs, const GroupID & rhs)
{
    return lhs.Equals(rhs);
}
inline bool operator != (const GroupID & lhs, const GroupID & rhs)
{
    return !lhs.Equals(rhs);
}
inline bool operator == (const __gid_t & lhs, const GroupID & rhs)
{
    return rhs.Equals(lhs);
}
inline bool operator != (const __gid_t & lhs, const GroupID & rhs)
{
    return !rhs.Equals(lhs);
}
inline bool operator == (const GroupID & lhs, const __gid_t & rhs)
{
    return lhs.Equals(rhs);
}
inline bool operator != (const GroupID & lhs, const __gid_t & rhs)
{
    return !lhs.Equals(rhs);
}

} // namespace Users
} // namespace OSAL
