#include "tcpSocketServer.h"

#include "clientHandler.h"
#include "exception.h"
#include "mysqlDatabase.h"

#include <iostream>
#include <unistd.h>
#include <sstream>

TCPSocketServer::TCPSocketServer(const int &port, std::shared_ptr<Database> database, std::shared_ptr<SentimentAnalyzer> analyzer)
    : port(port), database(database), analyzer(analyzer), running(false), serverFd(-1)
{
    initializeSocket();
    configureSocketOptions();
    configureServerAddress();
}

TCPSocketServer::~TCPSocketServer()
{
    disconnect();
}

void TCPSocketServer::initializeSocket()
{
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == 0)
    {
        throw NetworkConnectionException("Socket creation failed");
    }
}

void TCPSocketServer::configureSocketOptions()
{
    int opt = 1;
    if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0)
    {
        throw NetworkConnectionException("Set socket options failed");
    }
}

void TCPSocketServer::configureServerAddress()
{
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
}

bool TCPSocketServer::connect()
{
    if (bindSocket() && startListening())
    {
        acceptConnections();
        return true;
    }
    return false;
}

bool TCPSocketServer::bindSocket()
{
    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        throw NetworkConnectionException("Socket bind failed");
    }
    return true;
}

bool TCPSocketServer::startListening()
{
    if (listen(serverFd, MAX_CLIENTS) < 0)
    {
        throw NetworkConnectionException("Server is unable to listen for connections");
    }

    running = true;
    std::cout << "Server is listening on port " << port << std::endl;
    return true;
}

void TCPSocketServer::acceptConnections()
{
    while (running)
    {
        int addrlen = sizeof(address);
        int clientSocket = accept(serverFd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (clientSocket < 0)
        {
            std::cerr << "Failed to accept connection: " << strerror(errno) << std::endl;
            continue;
        }
        clientThreads.emplace_back(&TCPSocketServer::handleClient, this, clientSocket);
    }
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
    // Not directly used by the server
    return true;
}

std::string TCPSocketServer::receive()
{
    // Not directly used by the server
    return "";
}

void TCPSocketServer::handleClient(int clientSocket)
{
    ClientHandler handler(database, analyzer, clientSocket);
    handler.start();
}
