#pragma once

#include "database.h"
#include "networkConnection.h"
#include "sentimentAnalyzer.h"

#include <memory>
#include <netinet/in.h>
#include <thread>
#include <vector>

class TCPSocketServer : public NetworkConnection
{
private:
    struct sockaddr_in address;
    int serverFd;
    int port;
    bool running;
    std::vector<std::thread> clientThreads;
    static constexpr int MAX_DATA_TRANSFER_SIZE = 32768; // 2 ^ 15
    static constexpr int MAX_CLIENTS = 10;

    std::shared_ptr<Database> database;
    std::shared_ptr<SentimentAnalyzer> analyzer;

public:
    TCPSocketServer(const int &port, std::shared_ptr<Database> database, std::shared_ptr<SentimentAnalyzer> analyzer);
    ~TCPSocketServer();

    bool connect() override;
    bool disconnect() override;
    bool send(const std::string &message) override;
    std::string receive() override;

private:
    void initializeSocket();
    void configureSocketOptions();
    void configureServerAddress();
    bool bindSocket();
    bool startListening();
    void acceptConnections();
    void handleClient(int clientSocket);
};
