#include "tcpSocketServer.h"
#include "exception.h"
#include "mysqlDatabase.h"
#include "clientHandler.h"

#include <iostream>
#include <unistd.h>
#include <sstream>

TCPSocketServer::TCPSocketServer(const int &port, std::shared_ptr<Database> database, std::shared_ptr<SentimentAnalyzer> analyzer)
    : port(port), database(database), analyzer(analyzer), running(false)
{
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == 0)
    {
        throw NetworkConnectionException("Socket creation failed");
    }

    int opt = 1;
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        throw NetworkConnectionException("Set socket options failed");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
}

TCPSocketServer::~TCPSocketServer()
{
    disconnect();
}

bool TCPSocketServer::connect()
{
    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        throw NetworkConnectionException("Socket Bind failed");
    }

    if (listen(serverFd, MAX_CLIENTS) < 0)
    {
        throw NetworkConnectionException("Server is not able to listen to connections");
    }

    running = true;
    std::cout << "Server is listening on port " << port << std::endl;

    while (running)
    {
        int addrlen = sizeof(address);
        int clientSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (clientSocket < 0)
        {
            throw NetworkConnectionException("Server unable to accept connections");
        }
        clientThreads.emplace_back(&TCPSocketServer::handleClient, this, clientSocket);
    }

    return true;
}

bool TCPSocketServer::disconnect()
{
    running = false;
    close(serverFd);
    for (auto &thread : clientThreads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    return true;
}

bool TCPSocketServer::send(const std::string &message)
{
    // This method is not directly used by the server
    return true;
}

std::string TCPSocketServer::receive()
{
    // This method is not directly used by the server
    return "";
}

void TCPSocketServer::handleClient(int clientSocket)
{
    ClientHandler handler(database, analyzer, clientSocket);
    handler.start();
}
bool TCPSocketServer::updateRecommendRating(const std::string &itemId)
{
    int likes = 0;
    int dislikes = 0;
    int positiveSentiments = 0;
    int negativeSentiments = 0;
    int commentCount = 0;
    double likesWeight = 0.4, sentimentWeight = 0.4, commentWeight = 0.2;

    std::vector<std::vector<std::string>> resultSet;
    std::string query;

    query = "SELECT IFNULL(likes, 0), IFNULL(dislikes, 0) FROM Menu_Item WHERE item_id = " + itemId + "";
    resultSet = database->fetchRows(query);
    likes = std::stoi(resultSet[0][0]);
    dislikes = std::stoi(resultSet[0][1]);

    query = "SELECT IFNULL(COUNT(*), 0) FROM Menu_Item_Sentiment WHERE item_id = " + itemId + " AND type = 'positive'";
    resultSet = database->fetchRows(query);
    positiveSentiments = std::stoi(resultSet[0][0]);

    query = "SELECT IFNULL(COUNT(*), 0) FROM Menu_Item_Sentiment WHERE item_id = " + itemId + " AND type = 'negative'";
    resultSet = database->fetchRows(query);
    negativeSentiments = std::stoi(resultSet[0][0]);

    query = "SELECT IFNULL(COUNT(*), 0) FROM Comment WHERE item_id = " + itemId + "";
    resultSet = database->fetchRows(query);
    commentCount = std::stoi(resultSet[0][0]);

    double newRating = likesWeight * (likes - dislikes) + sentimentWeight * (positiveSentiments - negativeSentiments) + commentWeight * commentCount;

    query = "UPDATE Menu_Item SET recommend_rating = " + std::to_string(newRating) + " WHERE item_id = " + itemId;
    database->executeQuery(query);

    return true;
}