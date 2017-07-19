#include "unit-test-c++/UnitTestC++.h"
#include "network/Socket.h"
#include "core/Core.h"
#include <ifaddrs.h>

using namespace std;

namespace Network
{

namespace Test
{

class SocketTest : public UnitTestCpp::TestFixture
{
public:
    Socket target;

    virtual void SetUp();
    virtual void TearDown();
};

void SocketTest::SetUp()
{

}

void SocketTest::TearDown()
{
}

TEST_FIXTURE(SocketTest, Construct)
{
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_TRUE(target.IsClosed());
    string expected = "Network::Socket handle = -1";
    EXPECT_EQ(expected, target.ToString());
}

TEST_FIXTURE(SocketTest, ConstructCopy)
{
    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    Socket newSocket(target);
    EXPECT_EQ(handle, target.GetHandle());
    EXPECT_EQ(handle, newSocket.GetHandle());

    target.SetHandle(OSAL::Network::InvalidHandleValue);
    newSocket.SetHandle(OSAL::Network::InvalidHandleValue);
}

TEST_FIXTURE(SocketTest, ConstructMove)
{
    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    Socket newSocket(std::move(target));
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_EQ(handle, newSocket.GetHandle());

    target.SetHandle(OSAL::Network::InvalidHandleValue);
    newSocket.SetHandle(OSAL::Network::InvalidHandleValue);
}

TEST_FIXTURE(SocketTest, AssignCopy)
{
    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    Socket newSocket;

    newSocket = target;
    EXPECT_EQ(handle, target.GetHandle());
    EXPECT_EQ(handle, newSocket.GetHandle());

    target.SetHandle(OSAL::Network::InvalidHandleValue);
    newSocket.SetHandle(OSAL::Network::InvalidHandleValue);
}

TEST_FIXTURE(SocketTest, AssignMove)
{
    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    Socket newSocket;

    newSocket = std::move(target);
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_EQ(handle, newSocket.GetHandle());

    target.SetHandle(OSAL::Network::InvalidHandleValue);
    newSocket.SetHandle(OSAL::Network::InvalidHandleValue);
}

TEST_FIXTURE(SocketTest, GetSetHandle)
{
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());

    ostringstream stream;
    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    EXPECT_FALSE(target.IsClosed());
    stream << "Network::Socket handle = " << handle;
    string expected = stream.str();
    string actual = target.ToString();
    EXPECT_EQ(handle, target.GetHandle());
    EXPECT_EQ(expected, actual);

    stream.clear();
    stream.str("");
    stream << "Network::Socket handle = " << OSAL::Network::InvalidHandleValue;
    target.SetHandle(OSAL::Network::InvalidHandleValue);
    expected = stream.str();
    actual = target.ToString();
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(SocketTest, Open)
{
    target.Open(SocketFamily::Internet, SocketType::Stream);
    EXPECT_NE(OSAL::Network::InvalidHandleValue, target.GetHandle());
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_NE(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_FALSE(target.IsClosed());
}

TEST_FIXTURE(SocketTest, Close)
{
    target.Open(SocketFamily::Internet, SocketType::Stream);
    EXPECT_NE(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_FALSE(target.IsClosed());
    target.Close();
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_TRUE(target.IsClosed());
}

TEST_FIXTURE(SocketTest, GetOptionWithLevel)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    unsigned int size = sizeof(int);
    int value;
    target.GetSocketOption(SocketOptionLevel::Socket, SocketOption::Broadcast, &value, &size);
    EXPECT_FALSE(value != 0);
}

TEST_FIXTURE(SocketTest, SetOptionWithLevel)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(SocketOption::Broadcast));
    unsigned int size = sizeof(int);
    int value = 1;
    int actual;
    target.SetSocketOption(SocketOptionLevel::Socket, SocketOption::Broadcast, &value, size);
    target.GetSocketOption(SocketOptionLevel::Socket, SocketOption::Broadcast, &actual, &size);
    EXPECT_EQ(value, actual);
}

TEST_FIXTURE(SocketTest, GetOption)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    unsigned int size = sizeof(int);
    int value;
    target.GetSocketOption(SocketOption::Broadcast, &value, &size);
    EXPECT_FALSE(value != 0);
}

TEST_FIXTURE(SocketTest, SetOption)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(SocketOption::Broadcast));
    unsigned int size = sizeof(int);
    int value = 1;
    int actual;
    target.SetSocketOption(SocketOption::Broadcast, &value, size);
    target.GetSocketOption(SocketOption::Broadcast, &actual, &size);
    EXPECT_EQ(value, actual);
}

TEST_FIXTURE(SocketTest, GetSocketOptionBool)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(SocketOption::Broadcast));
}

TEST_FIXTURE(SocketTest, SetSocketOptionBool)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(SocketOption::Broadcast));
    target.SetSocketOptionBool(SocketOption::Broadcast, true);
    EXPECT_TRUE(target.GetSocketOptionBool(SocketOption::Broadcast));
}

TEST_FIXTURE(SocketTest, GetSocketOptionInt)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_EQ(0, target.GetSocketOptionInt(SocketOption::Broadcast));
}

TEST_FIXTURE(SocketTest, SetSocketOptionInt)
{
    int enableBroadcast = 1;
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionInt(SocketOption::Broadcast));
    target.SetSocketOptionInt(SocketOption::Broadcast, enableBroadcast);
    EXPECT_EQ(enableBroadcast, target.GetSocketOptionInt(SocketOption::Broadcast));
}

TEST_FIXTURE(SocketTest, GetBroadcastOption)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_FALSE(target.GetBroadcastOption());
}

TEST_FIXTURE(SocketTest, SetBroadcastOption)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_FALSE(target.GetBroadcastOption());
    target.SetBroadcastOption(true);
    EXPECT_TRUE(target.GetBroadcastOption());
}

TEST_FIXTURE(SocketTest, GetBlockingMode)
{
    target.Open(SocketFamily::Internet, SocketType::Stream);
    EXPECT_TRUE(target.GetBlockingMode());
}

TEST_FIXTURE(SocketTest, SetBlockingMode)
{
    target.Open(SocketFamily::Internet, SocketType::Stream);
    EXPECT_TRUE(target.GetBlockingMode());
    target.SetBlockingMode(false);
    EXPECT_FALSE(target.GetBlockingMode());
    target.SetBlockingMode(true);
    EXPECT_TRUE(target.GetBlockingMode());
}

TEST_FIXTURE(SocketTest, GetReuseAddress)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_FALSE(target.GetReuseAddress());
}

TEST_FIXTURE(SocketTest, SetReuseAddress)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_FALSE(target.GetReuseAddress());
    target.SetReuseAddress(true);
    EXPECT_TRUE(target.GetReuseAddress());
    target.SetReuseAddress(false);
    EXPECT_FALSE(target.GetReuseAddress());
}

TEST_FIXTURE(SocketTest, GetReceiveTimeout)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    int timeout = 0;
    EXPECT_EQ(timeout, target.GetReceiveTimeout());
}

TEST_FIXTURE(SocketTest, SetReceiveTimeout)
{
    int timeout = 0;
    int timeoutNew = 1000;
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_EQ(timeout, target.GetReceiveTimeout());
    target.SetReceiveTimeout(timeoutNew);
    EXPECT_EQ(timeoutNew, target.GetReceiveTimeout());
    target.SetReceiveTimeout(timeout);
    EXPECT_EQ(timeout, target.GetReceiveTimeout());
}

TEST_FIXTURE(SocketTest, GetSendTimeout)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    int timeout = 0;
    EXPECT_EQ(timeout, target.GetSendTimeout());
}

TEST_FIXTURE(SocketTest, SetSendTimeout)
{
    int timeout = 0;
    int timeoutNew = 1000;
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    EXPECT_EQ(timeout, target.GetSendTimeout());
    target.SetSendTimeout(timeoutNew);
    EXPECT_EQ(timeoutNew, target.GetSendTimeout());
    target.SetSendTimeout(timeout);
    EXPECT_EQ(timeout, target.GetSendTimeout());
}

TEST_FIXTURE(SocketTest, ToString)
{
    target.Open(SocketFamily::Internet, SocketType::Datagram);
    ostringstream stream;
    stream << "Network::Socket handle = " << target.GetHandle();
    EXPECT_EQ(stream.str(), target.ToString());
}

} // namespace Test

} // namespace Network
