#include <unittest-cpp/UnitTestC++.h>
#include "network/socket.h"
#include "core/Core.h"
//#include <ifaddrs.h>

using namespace std;

namespace Network
{

namespace Test
{

class socketTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp() override
    {
        int errorCode = OSAL::Network::InitializeSocketLibrary();
        if (errorCode != 0)
        {
            cout << OSAL::Network::GetSocketErrorMessage(errorCode);
        }
    }
    virtual void TearDown() override
    {
        int errorCode = OSAL::Network::UninitializeSocketLibrary();
        if (errorCode != 0)
        {
            cout << OSAL::Network::GetSocketErrorMessage(errorCode);
        }
    }
};

TEST_SUITE(network)
{

TEST_FIXTURE(socketTest, Construct)
{
    socket target;

    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_TRUE(target.IsClosed());
    std::string expected = "Network::socket handle = -1";
    std::ostringstream stream;
    target.PrintTo(stream);
    EXPECT_EQ(expected, stream.str());
}

TEST_FIXTURE(socketTest, ConstructCopy)
{
    socket target;

    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    socket newSocket(target);
    EXPECT_EQ(handle, target.GetHandle());
    EXPECT_EQ(handle, newSocket.GetHandle());

    target.SetHandle(OSAL::Network::InvalidHandleValue);
    newSocket.SetHandle(OSAL::Network::InvalidHandleValue);
}

TEST_FIXTURE(socketTest, ConstructMove)
{
    socket target;

    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    socket newSocket(std::move(target));
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_EQ(handle, newSocket.GetHandle());

    target.SetHandle(OSAL::Network::InvalidHandleValue);
    newSocket.SetHandle(OSAL::Network::InvalidHandleValue);
}

TEST_FIXTURE(socketTest, AssignCopy)
{
    socket target;

    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    socket newSocket;

    newSocket = target;
    EXPECT_EQ(handle, target.GetHandle());
    EXPECT_EQ(handle, newSocket.GetHandle());

    target.SetHandle(OSAL::Network::InvalidHandleValue);
    newSocket.SetHandle(OSAL::Network::InvalidHandleValue);
}

TEST_FIXTURE(socketTest, AssignMove)
{
    socket target;

    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    socket newSocket;

    newSocket = std::move(target);
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_EQ(handle, newSocket.GetHandle());

    target.SetHandle(OSAL::Network::InvalidHandleValue);
    newSocket.SetHandle(OSAL::Network::InvalidHandleValue);
}

TEST_FIXTURE(socketTest, GetSetHandle)
{
    socket target;

    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());

    std::ostringstream streamExpected;
    OSAL::Network::SocketHandle handle = 1234;
    target.SetHandle(handle);
    EXPECT_FALSE(target.IsClosed());
    streamExpected << "Network::socket handle = " << handle;
    std::string expected = streamExpected.str();
    std::ostringstream streamActual;
    target.PrintTo(streamActual);
    EXPECT_EQ(handle, target.GetHandle());
    EXPECT_EQ(expected, streamActual.str());

    streamExpected.clear();
    streamExpected.str("");
    streamExpected << "Network::socket handle = " << static_cast<long>(OSAL::Network::InvalidHandleValue);
    target.SetHandle(OSAL::Network::InvalidHandleValue);
    expected = streamExpected.str();
    streamActual.clear();
    streamActual.str("");
    target.PrintTo(streamActual);
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_EQ(expected, streamActual.str());
}

TEST_FIXTURE(socketTest, Open)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Stream);
    EXPECT_NE(OSAL::Network::InvalidHandleValue, target.GetHandle());
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_NE(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_FALSE(target.IsClosed());
}

TEST_FIXTURE(socketTest, Close)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Stream);
    EXPECT_NE(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_FALSE(target.IsClosed());
    target.Close();
    EXPECT_EQ(OSAL::Network::InvalidHandleValue, target.GetHandle());
    EXPECT_TRUE(target.IsClosed());
}

TEST_FIXTURE(socketTest, GetOptionWithLevel)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    socklen_t size = sizeof(int);
    int value;
    target.GetSocketOption(OSAL::Network::SocketOptionLevel::Socket, OSAL::Network::SocketOption::Broadcast, &value, size);
    EXPECT_FALSE(value != 0);
}

TEST_FIXTURE(socketTest, SetOptionWithLevel)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast));
    socklen_t size = sizeof(int);
    int value = 1;
    int actual;
    target.SetSocketOption(OSAL::Network::SocketOptionLevel::Socket, OSAL::Network::SocketOption::Broadcast, &value, size);
    target.GetSocketOption(OSAL::Network::SocketOptionLevel::Socket, OSAL::Network::SocketOption::Broadcast, &actual, size);
    EXPECT_EQ(value, actual);
}

TEST_FIXTURE(socketTest, GetOption)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    socklen_t size = sizeof(int);
    int value;
    target.GetSocketOption(OSAL::Network::SocketOption::Broadcast, &value, size);
    EXPECT_FALSE(value != 0);
}

TEST_FIXTURE(socketTest, SetOption)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast));
    socklen_t size = sizeof(int);
    int value = 1;
    int actual;
    target.SetSocketOption(OSAL::Network::SocketOption::Broadcast, &value, size);
    target.GetSocketOption(OSAL::Network::SocketOption::Broadcast, &actual, size);
    EXPECT_EQ(value, actual);
}

TEST_FIXTURE(socketTest, GetSocketOptionBool)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast));
}

TEST_FIXTURE(socketTest, SetSocketOptionBool)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast));
    target.SetSocketOptionBool(OSAL::Network::SocketOption::Broadcast, true);
    EXPECT_TRUE(target.GetSocketOptionBool(OSAL::Network::SocketOption::Broadcast));
}

TEST_FIXTURE(socketTest, GetSocketOptionInt)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_EQ(0, target.GetSocketOptionInt(OSAL::Network::SocketOption::Broadcast));
}

TEST_FIXTURE(socketTest, SetSocketOptionInt)
{
    socket target;

    int enableBroadcast = 1;
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetSocketOptionInt(OSAL::Network::SocketOption::Broadcast));
    target.SetSocketOptionInt(OSAL::Network::SocketOption::Broadcast, enableBroadcast);
    EXPECT_EQ(enableBroadcast, target.GetSocketOptionInt(OSAL::Network::SocketOption::Broadcast));
}

TEST_FIXTURE(socketTest, GetBroadcastOption)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetBroadcastOption());
}

TEST_FIXTURE(socketTest, SetBroadcastOption)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetBroadcastOption());
    target.SetBroadcastOption(true);
    EXPECT_TRUE(target.GetBroadcastOption());
}

TEST_FIXTURE(socketTest, GetBlockingMode)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Stream);
    EXPECT_TRUE(target.GetBlockingMode());
}

TEST_FIXTURE(socketTest, SetBlockingMode)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Stream);
    EXPECT_TRUE(target.GetBlockingMode());
    target.SetBlockingMode(false);
    EXPECT_FALSE(target.GetBlockingMode());
    target.SetBlockingMode(true);
    EXPECT_TRUE(target.GetBlockingMode());
}

TEST_FIXTURE(socketTest, GetReuseAddress)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetReuseAddress());
}

TEST_FIXTURE(socketTest, SetReuseAddress)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_FALSE(target.GetReuseAddress());
    target.SetReuseAddress(true);
    EXPECT_TRUE(target.GetReuseAddress());
    target.SetReuseAddress(false);
    EXPECT_FALSE(target.GetReuseAddress());
}

TEST_FIXTURE(socketTest, GetReceiveTimeout)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    int timeout = 0;
    EXPECT_EQ(timeout, target.GetReceiveTimeout());
}

TEST_FIXTURE(socketTest, SetReceiveTimeout)
{
    socket target;

    int timeout = 0;
    int timeoutNew = 1000;
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_EQ(timeout, target.GetReceiveTimeout());
    target.SetReceiveTimeout(timeoutNew);
    EXPECT_EQ(timeoutNew, target.GetReceiveTimeout());
    target.SetReceiveTimeout(timeout);
    EXPECT_EQ(timeout, target.GetReceiveTimeout());
}

TEST_FIXTURE(socketTest, GetSendTimeout)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    int timeout = 0;
    EXPECT_EQ(timeout, target.GetSendTimeout());
}

TEST_FIXTURE(socketTest, SetSendTimeout)
{
    socket target;

    int timeout = 0;
    int timeoutNew = 1000;
    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    EXPECT_EQ(timeout, target.GetSendTimeout());
    target.SetSendTimeout(timeoutNew);
    EXPECT_EQ(timeoutNew, target.GetSendTimeout());
    target.SetSendTimeout(timeout);
    EXPECT_EQ(timeout, target.GetSendTimeout());
}

TEST_FIXTURE(socketTest, PrintTo)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    std::ostringstream streamExpected;
    streamExpected << "Network::socket handle = " << target.GetHandle();
    std::ostringstream streamActual;
    target.PrintTo(streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

TEST_FIXTURE(socketTest, PrintToNonMember)
{
    socket target;

    target.Open(OSAL::Network::SocketFamily::Internet, OSAL::Network::SocketType::Datagram);
    std::ostringstream streamExpected;
    streamExpected << "Network::socket handle = " << target.GetHandle();
    std::ostringstream streamActual;
    PrintTo(target, streamActual);

    EXPECT_EQ(streamExpected.str(), streamActual.str());
}

} // TEST_SUITE(network)

} // namespace Test
} // namespace Network
