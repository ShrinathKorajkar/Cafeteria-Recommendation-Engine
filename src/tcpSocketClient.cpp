#include "tcpSocketClient.h"
#include "exception.h"

#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>

std::shared_ptr<TCPSocketClient> TCPSocketClient::instance = nullptr;

TCPSocketClient::TCPSocketClient(const std::string &serverAddress, int port)
    : serverAddress(serverAddress), port(port), clientSocket(-1) {}

TCPSocketClient::~TCPSocketClient()
{
    disconnect();
}

void TCPSocketClient::createInstance(const std::string &serverAddress, int port)
{
    if (!instance)
    {
        instance = std::shared_ptr<TCPSocketClient>(new TCPSocketClient(serverAddress, port));
    }
}
std::shared_ptr<TCPSocketClient> TCPSocketClient::getInstance()
{
    if (!instance)
    {
        throw std::runtime_error("TCPSocketClient instance has not been created. Call createInstance first.");
    }
    return instance;
}

bool TCPSocketClient::connect()
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        throw NetworkConnectionException("Socket creation failed");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0)
    {
        throw NetworkConnectionException("Invalid address/Address not supported");
    }

    if (::connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        throw NetworkConnectionException("Connection failed");
    }

    return true;
}

bool TCPSocketClient::disconnect()
{
    if (clientSocket != -1)
    {
        send("CLOSE_CONNECTION");
    }
    close(clientSocket);
    clientSocket = -1;
    return true;
}

bool TCPSocketClient::send(const std::string &message)
{
    std::cout << "Sending: " << message << std::endl;
    if (::send(clientSocket, message.c_str(), message.size(), 0) < 0)
    {
        throw NetworkConnectionException("Failed to send data to server");
    }

    return true;
}

std::string TCPSocketClient::receive()
{
    char buffer[MAX_DATA_TRANSFER_SIZE] = {0};
    int bytesRead = read(clientSocket, buffer, MAX_DATA_TRANSFER_SIZE);
    if (bytesRead < 0)
    {
        throw NetworkConnectionException("Failed to receive data from server");
    }
    return std::string(buffer, bytesRead);
}
