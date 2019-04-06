#pragma once

#include <sscfts1.h>

#include "ITest.h"

class TestClient : public SscfTs1::EventHandler
{
public:
    TestClient();
    TestClient(const TestClient &) = delete;
    TestClient & operator=(const TestClient &) = delete;

    ~TestClient();

    void Connect(const std::string & network);

    void Disconnect();

    bool IsConnected() const;

    int Start();

    int Stop();

    //events
    void OnConnectionLost();
    void OnCallback();

private:
    SscfTs1::EventLoop * eventLoop_;
    SscfTs1::ClientNode * client_;
    std::unique_ptr<ITestProxy> proxy_;

    bool isConnected_;
};
