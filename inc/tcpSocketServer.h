#ifndef SERVER_H
#define SERVER_H

#include "networkConnection.h"
#include "database.h"

#include <memory>
#include <netinet/in.h>
#include <vector>
#include <thread>

class TCPSocketServer : public NetworkConnection
{
private:
    struct sockaddr_in address;
    int serverFd;
    int port;
    bool running;
    std::vector<std::thread> clientThreads;
    int MAX_DATA_TRANSFER_SIZE = 1024;
    int MAX_CLIENTS = 10;

    std::shared_ptr<Database> database;

    void handleClient(int clientSocket);

public:
    TCPSocketServer(const int &port, std::shared_ptr<Database> database);
    ~TCPSocketServer();
    bool connect() override;
    bool disconnect() override;
    bool send(const std::string &message) override;
    std::string receive() override;
};

#endif // SERVER_H
