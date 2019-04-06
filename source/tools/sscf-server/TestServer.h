#pragma once

#include <sscfts1.h>
#include <iostream>
#include <thread>

#include "ITest.h"

class TestServer : public ITest
{
public:
    TestServer(SscfTs1::EventLoop * eventLoop)
            : eventLoop_(eventLoop)
    {}
    int Start() override
    {
        std::cout << "BEG TestServer::Start" << std::endl;
        stopThread_ = false;
        callbackThread_ = std::move(std::thread(&TestServer::CallbackThread, this));
        OnCallback();
        std::cout << "END TestServer::Start" << std::endl;
        return 1;
    }
    int Stop() override
    {
        std::cout << "BEG TestServer::Stop" << std::endl;
        OnCallback();
        stopThread_ = true;
        std::cout << "Wait for thread" << std::endl;
        callbackThread_.join();
        std::cout << "END TestServer::Stop" << std::endl;
        return 0;
    }

    void OnCallback()
    {
        std::cout << "BEG TestServer::OnCallback" << std::endl;
        eventLoop_->post([this]
                         {
                             Callback();
                         });
        std::cout << "END TestServer::OnCallback" << std::endl;
    }

protected:
    std::thread callbackThread_;
    bool stopThread_;
    SscfTs1::EventLoop * eventLoop_;

    void CallbackThread()
    {
        // std::cout << "BEG TestServer::CallbackThread" << std::endl;
        while (!stopThread_)
        {
            //OnCallback();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        // std::cout << "END TestServer::CallbackThread" << std::endl;
    }
};

