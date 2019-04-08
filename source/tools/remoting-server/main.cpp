#include <csignal>   // signal()
#include <iostream>
#include <memory>
#include <thread> // this_thread, std::thread, std::chrono
//#include "GlobalFunctions.h"
#include "remotingts1/eventloop.h"
#include "remotingts1/servernode.h"
#include "util.h"

#include <osal/Unused.h>
#include "TestServer.h"
#include "ITestDefinition.h"

// namespace std {
// template<class T, class... Args>
//     std::unique_ptr<T> make_unique(Args&&... args)
//     {
//         return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
//     }
// }

template<class Implementation>
class RemotingServer
{
public:
    RemotingServer()
        : implementation_()
        , ipAddress_()
        , serverNode_()
        , eventLoop_()
    {
    }

    virtual ~RemotingServer()
    {
        if (eventLoop_)
        {
            std::cout << "Delete eventLoop" << std::endl;
            eventLoop_.reset();
        }
        if (implementation_)
        {
            std::cout << "Delete implementation" << std::endl;
            implementation_.reset();
        }
    }

    void StartServer(const std::string & ipAddress)
    {
        std::cout << "BEG RemotingServer::StartServer" << std::endl;
        ipAddress_ = ipAddress;
        
        eventLoop_ = std::make_unique<RemotingTs1::EventLoop>(ITestUrn);
        implementation_ = std::make_unique<Implementation>(eventLoop_.get());
        eventLoop_->send([this]
        {
            serverNode_ = std::make_unique<RemotingTs1::ServerNode>(*eventLoop_, ipAddress_, ITestUrn);
            serverNode_->registerSingleton<ITest>(implementation_.get());
            // serverNode->registerSingleton<AvailabilityProvider::IAvailabilityProvider>(clientProxy);
            serverNode_->listen();
        });
        std::cout << "END RemotingServer::StartServer" << std::endl;
    }
    void RunServerSync()
    {
        std::cout << "BEG RemotingServer::RunServerSync" << std::endl;
        eventLoop_->run();
        std::cout << "END RemotingServer::RunServerSync" << std::endl;
    }
    void RunServerAsync()
    {
        std::cout << "BEG RemotingServer::RunServerAsync" << std::endl;
        eventLoop_->start();
        std::cout << "END RemotingServer::RunServerAsync" << std::endl;
    }
    bool IsServerRunning()
    {
        return eventLoop_->running();
    }
    void StopServer()
    {
        if (eventLoop_ == nullptr)
            return;
        std::cout << "BEG RemotingServer::StopServer" << std::endl;
        eventLoop_->send([this]
        {
            if (serverNode_)
            {
                std::cout << "Delete serverNode" << std::endl;
                serverNode_.reset();
            }
        });
        std::cout << "Server stop" << std::endl;
        eventLoop_->stop(0);
        std::cout << "Server stopping" << std::endl;
        std::cout << "END RemotingServer::StopServer" << std::endl;
    }

private:
    std::unique_ptr<Implementation> implementation_;
    std::string ipAddress_;
    std::unique_ptr<RemotingTs1::ServerNode> serverNode_;
    std::unique_ptr<RemotingTs1::EventLoop> eventLoop_;
};

class Application
{
public:
    Application()
        : server_()
        , shouldStop_()
    {
    }

    void Start(const std::string & ipAddress)
    {
        std::cout << "BEG Application::Start" << std::endl;
        server_.StartServer(ipAddress);
        std::cout << "END Application::Start" << std::endl;
    }
    void Run()
    {
        server_.RunServerAsync();
    }
    void RequestStop()
    {
        std::cout << "BEG Application::RequestStop" << std::endl;
        server_.StopServer();
        shouldStop_ = true;
        std::cout << "END Application::RequestStop" << std::endl;
    }
    bool ShouldStop()
    {
        return shouldStop_;
    }
    void Stop()
    {
        std::cout << "BEG Application::Stop" << std::endl;
        std::cout << "END Application::Stop" << std::endl;
    }

private:
    RemotingServer<TestServer> server_;
    bool shouldStop_;
};

Application app;

#ifdef __linux__
void OnCtrlC(int signal, siginfo_t * info, void * data)
{
    std::cout << "Ctrl-C was pressed" << std::endl;
    app.RequestStop();
}
#endif

int main(int UNUSED(argc), char * UNUSED(argv)[])
{
#ifdef __linux__
    const std::string ipAddress = GetIpAddress("eth0");
#else
    const std::string ipAddress = "127.0.0.1";
#endif

    std::cout << "Starting" << std::endl;
    app.Start(ipAddress);
    app.Run();
#ifdef __linux__
    struct sigaction signalHandler {};
	signalHandler.sa_sigaction = OnCtrlC;
	signalHandler.sa_flags = SA_SIGINFO;
	struct sigaction signalHandlerOld;
	sigaction(SIGINT, &signalHandler, &signalHandlerOld);
#endif
    std::cout << "Running, press any key to stop" << std::endl;
    while (!app.ShouldStop())
    {
        if (GetCh() != 0)
            app.RequestStop();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "Stopping" << std::endl;
    app.Stop();
    std::cout << "Stopped" << std::endl;
	return 0;
}