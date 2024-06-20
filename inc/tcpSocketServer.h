#pragma once

#include "networkConnection.h"
#include "database.h"
#include "sentimentAnalyzer.h"
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
    static constexpr int MAX_DATA_TRANSFER_SIZE = 32768;
    static constexpr int MAX_CLIENTS = 10;

    std::shared_ptr<Database> database;
    std::shared_ptr<SentimentAnalyzer> analyzer;

    void handleClient(int clientSocket);
    bool updateRecommendRating(const std::string &itemId);

public:
    TCPSocketServer(const int &port, std::shared_ptr<Database> database, std::shared_ptr<SentimentAnalyzer> analyzer);
    ~TCPSocketServer();

    bool connect() override;
    bool disconnect() override;
    bool send(const std::string &message) override;
    std::string receive() override;
};
