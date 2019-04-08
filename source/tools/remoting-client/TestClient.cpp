#include "TestClient.h"

#include <iostream>
#include <chrono>
#include <thread>
// #include <locale>
// #include <codecvt>
// #include <string>
#include <osal/Unused.h>

#include "ITestDefinition.h"

TestClient::TestClient()
    : eventLoop_()
    , client_()
    , proxy_()
    , isConnected_()
{
    // TRACE_FUNC_BEGIN("");
    // TRACE_FUNC_END("");
}

TestClient::~TestClient()
{
    // TRACE_FUNC_BEGIN("");
    eventLoop_->send([this]
    {
        proxy_.reset(nullptr);
        delete client_;
        client_ = nullptr;
    });
    delete eventLoop_;
    eventLoop_ = nullptr;
    // TRACE_FUNC_END("");
}

void TestClient::Connect(const std::string & network)
{
    static bool executed = false;

    std::cout << "BEG TestClient::Connect (" << "executed=" << executed << ")" << std::endl;
    // TRACE_FUNC_BEGIN("executed=" + STR(executed));

    eventLoop_ = new RemotingTs1::EventLoop("Test");
    eventLoop_->start();
    eventLoop_->send([this, &network]
    {
        client_ = new RemotingTs1::ClientNode(*eventLoop_, network, ITestUrn);
    });

    unsigned count(10u); //Pick a reasonable number
    eventLoop_->send([this, &count]
    {
        std::cout << "Connecting to server ";

        while(!client_->connect() && count != 0u)
        {
            std::cout << ".";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            count--;
        }
        std::cout << std::endl;
    });

    if (count == 0)
    {
        const std::string msg = std::string("Unable to connect ") + __FUNCTION__;
        // TRACE_FUNC(msg);
        std::cout << msg << std::endl;
        // throw CControllerException("TestClient::Connect - " + msg);
    }
    // TRACE_FUNC("Connected");
    std::cout << "Connected" << std::endl;

    isConnected_ = {};

    eventLoop_->send([this]
    {
        std::cout << "Create new proxy" << std::endl;
        proxy_.reset(new ITestProxy(*client_)); // assignment to new pointer.
        // TRACE_FUNC("New FDC proxy created");
        std::cout << "New FDC proxy created" << std::endl;
        subscribe(proxy_->Callback, &TestClient::OnCallback);

        if (!executed)
        {
            subscribe(client_->disconnected, &TestClient::OnConnectionLost);
            executed = true;
        }
        isConnected_ = client_->isConnected();
    });

    std::cout << "END TestClient::Connect (" << "isConnected=" << isConnected_ << " executed=" << executed << ")" << std::endl;
    // TRACE_FUNC_END("isConnected=" + STR(isConnected_) + " executed=" + STR(executed));
}

void TestClient::Disconnect()
{
    std::cout << "BEG TestClient::Disconnect" << std::endl;
    // TRACE_FUNC_BEGIN("");

    eventLoop_->send([this]
    {
        client_->disconnect();
        isConnected_ = client_->isConnected();
    });
    // TRACE_FUNC_END("isConnected=" + STR(isConnected_));
    std::cout << "END TestClient::Disconnect (" << "isConnected=" << isConnected_ << ")" << std::endl;
}

bool TestClient::IsConnected() const
{
    std::cout << "BEG TestClient::IsConnected" << std::endl;
    std::cout << "END TestClient::IsConnected = " << isConnected_ << std::endl;
    return isConnected_;
}

int TestClient::Start()
{
    std::cout << "BEG TestClient::Start" << std::endl;
    int result {};
    try
    {
        result = proxy_->Start();
    }
    catch (const std::exception & UNUSED(e))
    { 
        result = -1;
    }
    std::cout << "END TestClient::Start = " << result << std::endl;
    return result;
}

int TestClient::Stop()
{
    std::cout << "BEG TestClient::Stop" << std::endl;
    int result {};
    try
    {
        result = proxy_->Stop();
    }
    catch (const std::exception & UNUSED(e))
    { 
        result = -1;
    }
    std::cout << "END TestClient::Stop = " << result << std::endl;
    return result;
}

void TestClient::OnConnectionLost()
{
    std::cout << "<---------- OnConnectionLost received by TestClient" << std::endl;

    // TRACE_FUNC_BEGIN("");
    isConnected_ = {};
}

void TestClient::OnCallback()
{
    std::cout << "Callback" << std::endl;
}
