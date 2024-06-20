#ifndef CLIENT_H
#define CLIENT_H

#include "networkConnection.h"
#include <memory>

#include <netinet/in.h>

class TCPSocketClient : public NetworkConnection
{
private:
    struct sockaddr_in serverAddr;
    std::string serverAddress;
    int port;
    int clientSocket;
    int MAX_DATA_TRANSFER_SIZE = 32768;
    TCPSocketClient(const std::string &serverAddress, int port);

    static std::shared_ptr<TCPSocketClient> instance;

public:
    static void createInstance(const std::string &serverAddress, int port);
    static std::shared_ptr<TCPSocketClient> getInstance();
    ~TCPSocketClient();
    bool connect() override;
    bool disconnect() override;
    bool send(const std::string &message) override;
    std::string receive() override;

    // std::stringstream serializeMessage();
    // std::string deserializeMessage(std::stringstream &message);
};

#endif // CLIENT_H
