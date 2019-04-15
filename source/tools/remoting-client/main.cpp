#include <iostream>
#include <thread>
#include <osal/Unused.h>
#include "TestClient.h"
#include "util.h"

// namespace std {
// template<class T, class... Args>
//     std::unique_ptr<T> make_unique(Args&&... args)
//     {
//         return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
//     }
// }

template<class Implementation>
class REMOTINGClient
{
public:
    REMOTINGClient()
        : ipAddress_()
        , proxy_()
        , isConnected_()
    {
    }

    virtual ~REMOTINGClient()
    {
        if (proxy_)
        {
            std::cout << "Delete proxy" << std::endl;
            proxy_.reset();
        }
    }

    bool Connect(const std::string & ipAddress)
    {
        std::cout << "BEG REMOTINGClient::Connect" << std::endl;
        ipAddress_ = ipAddress;
        proxy_ = std::make_unique<Implementation>();
        
        proxy_->Connect(ipAddress);
        isConnected_ = proxy_->IsConnected();
        std::cout << "END REMOTINGClient::Connect" << std::endl;
        return isConnected_;
    }
    void Disconnect()
    {
        std::cout << "BEG REMOTINGClient::Start" << std::endl;
        proxy_->Disconnect();
        std::cout << "END REMOTINGClient::Start" << std::endl;
    }
    bool IsConnected()
    {
        std::cout << "BEG REMOTINGClient::IsConnected" << std::endl;
        auto result = proxy_->IsConnected();
        std::cout << "END REMOTINGClient::IsConnected" << std::endl;
        return result;
    }

    int Start()
    {
        std::cout << "BEG REMOTINGClient::Start" << std::endl;
        auto result = proxy_->Start();
        std::cout << "END REMOTINGClient::Start" << std::endl;
        return result;
    }

    int Stop()
    {
        std::cout << "BEG REMOTINGClient::Stop" << std::endl;
        auto result = proxy_->Stop();
        std::cout << "END REMOTINGClient::Stop" << std::endl;
        return result;
    }

private:
    std::string ipAddress_;
    std::unique_ptr<Implementation> proxy_;
    bool isConnected_;
};

REMOTINGClient<TestClient> client;

int main(int UNUSED(argc), char ** UNUSED(argv))
{
#ifdef __linux__
    const std::string ipAddress = GetIpAddress("eth0");
#else
    const std::string ipAddress = "127.0.0.1";
#endif

    std::cout << "Connect" << std::endl;
    std::cout << client.Connect(ipAddress) << std::endl;
    std::cout << "Start" << std::endl;
    std::cout << client.Start() << std::endl;
    std::cout << "Stop" << std::endl;
    std::cout << client.Stop() << std::endl;
    std::cout << "Disconnect" << std::endl;
    client.Disconnect();
	return 0;
}