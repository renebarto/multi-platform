#include <unittest-c++/UnitTestC++.h>
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

TEST_SUITE(network)
{

TEST_FIXTURE(SocketTest, Construct)
{
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_TRUE(target.IsClosed());
    OSAL::String expected = _("Network::Socket handle = -1");
    std::basic_ostringstream<OSAL::Char> stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
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

    std::basic_ostringstream<OSAL::Char> streamExpected;
    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    EXPECT_FALSE(target.IsClosed());
    streamExpected << _("Network::Socket handle = ") << handle;
    OSAL::String expected = streamExpected.str();
    std::basic_ostringstream<OSAL::Char> streamActual;
    target.PrintTo(streamActual);
    EXPECT_EQ(handle, target.GetHandle());
    EXPECT_EQ(expected, streamActual.str());

    streamExpected.clear();
    streamExpected.str(_(""));
    streamExpected << _("Network::Socket handle = ") << OSAL::Network::InvalidHandleValue;
    target.SetHandle(OSAL::Network::InvalidHandleValue);
    expected = streamExpected.str();
    streamActual.clear();
    streamActual.str("");
    target.PrintTo(streamActual);
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_EQ(expected, streamActual.str());
}

TEST_FIXTURE(SocketTest, Open)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Stream);
    EXPECT_NE(OSAL::Network::InvalidHandleValue, target.GetHandle());
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_NE(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_FALSE(target.IsClosed());
}

TEST_FIXTURE(SocketTest, Close)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Stream);
    EXPECT_NE(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_FALSE(target.IsClosed());
    target.Close();
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_TRUE(target.IsClosed());
}

TEST_FIXTURE(SocketTest, GetOptionWithLevel)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    unsigned int size = sizeof(int);
    int value;
    target.GetSocketOption(OSAL::Network::SocketOptionLevel::Socket, OSAL::Network::SocketOption::Broadcast, &value, size);
    EXPECT_FALSE(value != 0);
}

TEST_FIXTURE(SocketTest, SetOptionWithLevel)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast));
    unsigned int size = sizeof(int);
    int value = 1;
    int actual;
    target.SetSocketOption(OSAL::Network::SocketOptionLevel::Socket, OSAL::Network::SocketOption::Broadcast, &value, size);
    target.GetSocketOption(OSAL::Network::SocketOptionLevel::Socket, OSAL::Network::SocketOption::Broadcast, &actual, size);
    EXPECT_EQ(value, actual);
}

TEST_FIXTURE(SocketTest, GetOption)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    unsigned int size = sizeof(int);
    int value;
    target.GetSocketOption(OSAL::Network::SocketOption::Broadcast, &value, size);
    EXPECT_FALSE(value != 0);
}

TEST_FIXTURE(SocketTest, SetOption)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast));
    unsigned int size = sizeof(int);
    int value = 1;
    int actual;
    target.SetSocketOption(OSAL::Network::SocketOption::Broadcast, &value, size);
    target.GetSocketOption(OSAL::Network::SocketOption::Broadcast, &actual, size);
    EXPECT_EQ(value, actual);
}

TEST_FIXTURE(SocketTest, GetSocketOptionBool)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast));
}

TEST_FIXTURE(SocketTest, SetSocketOptionBool)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast));
    target.SetSocketOptionBool(OSAL::Network::SocketOption::Broadcast, true);
    EXPECT_TRUE(target.GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast));
}

TEST_FIXTURE(SocketTest, GetSocketOptionInt)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_EQ(0, target.GetSocketOptionInt(OSAL::Network::SocketOption::Broadcast));
}

TEST_FIXTURE(SocketTest, SetSocketOptionInt)
{
    int enableBroadcast = 1;
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionInt(OSAL::Network::SocketOption::Broadcast));
    target.SetSocketOptionInt(OSAL::Network::SocketOption::Broadcast, enableBroadcast);
    EXPECT_EQ(enableBroadcast, target.GetSocketOptionInt(OSAL::Network::SocketOption::Broadcast));
}

TEST_FIXTURE(SocketTest, GetBroadcastOption)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetBroadcastOption());
}

TEST_FIXTURE(SocketTest, SetBroadcastOption)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetBroadcastOption());
    target.SetBroadcastOption(true);
    EXPECT_TRUE(target.GetBroadcastOption());
}

TEST_FIXTURE(SocketTest, GetBlockingMode)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Stream);
    EXPECT_TRUE(target.GetBlockingMode());
}

TEST_FIXTURE(SocketTest, SetBlockingMode)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Stream);
    EXPECT_TRUE(target.GetBlockingMode());
    target.SetBlockingMode(false);
    EXPECT_FALSE(target.GetBlockingMode());
    target.SetBlockingMode(true);
    EXPECT_TRUE(target.GetBlockingMode());
}

TEST_FIXTURE(SocketTest, GetReuseAddress)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetReuseAddress());
}

TEST_FIXTURE(SocketTest, SetReuseAddress)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetReuseAddress());
    target.SetReuseAddress(true);
    EXPECT_TRUE(target.GetReuseAddress());
    target.SetReuseAddress(false);
    EXPECT_FALSE(target.GetReuseAddress());
}

TEST_FIXTURE(SocketTest, GetReceiveTimeout)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    int timeout = 0;
    EXPECT_EQ(timeout, target.GetReceiveTimeout());
}

TEST_FIXTURE(SocketTest, SetReceiveTimeout)
{
    int timeout = 0;
    int timeoutNew = 1000;
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_EQ(timeout, target.GetReceiveTimeout());
    target.SetReceiveTimeout(timeoutNew);
    EXPECT_EQ(timeoutNew, target.GetReceiveTimeout());
    target.SetReceiveTimeout(timeout);
    EXPECT_EQ(timeout, target.GetReceiveTimeout());
}

TEST_FIXTURE(SocketTest, GetSendTimeout)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    int timeout = 0;
    EXPECT_EQ(timeout, target.GetSendTimeout());
}

TEST_FIXTURE(SocketTest, SetSendTimeout)
{
    int timeout = 0;
    int timeoutNew = 1000;
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_EQ(timeout, target.GetSendTimeout());
    target.SetSendTimeout(timeoutNew);
    EXPECT_EQ(timeoutNew, target.GetSendTimeout());
    target.SetSendTimeout(timeout);
    EXPECT_EQ(timeout, target.GetSendTimeout());
}

TEST_FIXTURE(SocketTest, PrintTo)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    std::basic_ostringstream<OSAL::Char> streamExpected;
    streamExpected << _("Network::Socket handle = ") << target.GetHandle();
    std::basic_ostringstream<OSAL::Char> streamActual;
    target.PrintTo(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

TEST_FIXTURE(SocketTest, PrintToNonMember)
{
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    std::basic_ostringstream<OSAL::Char> streamExpected;
    streamExpected << _("Network::Socket handle = ") << target.GetHandle();
    std::basic_ostringstream<OSAL::Char> streamActual;
    PrintTo(target, streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

} // TEST_SUITE(network)

} // namespace Test
} // namespace Network
