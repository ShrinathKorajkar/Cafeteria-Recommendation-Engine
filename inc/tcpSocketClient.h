#pragma once

#include "networkConnection.h"
#include "utility.h"
#include <memory>
#include <netinet/in.h>
#include <string>

class TCPSocketClient : public NetworkConnection
{
private:
    struct sockaddr_in serverAddr;
    std::string serverAddress;
    int port;
    int clientSocket;
    static constexpr int MAX_DATA_TRANSFER_SIZE = 32768;

    static std::shared_ptr<TCPSocketClient> instance;

    TCPSocketClient(const std::string &serverAddress, int port);

public:
    static void createInstance(const std::string &serverAddress, int port);
    static std::shared_ptr<TCPSocketClient> getInstance();
    ~TCPSocketClient();

    bool connect() override;
    bool disconnect() override;
    bool send(const std::string &message) override;
    std::string receive() override;
};
