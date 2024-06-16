#include "tcpSocketServer.h"
#include "exception.h"
#include "mysqlDatabase.h"

#include <iostream>
#include <unistd.h>
#include <sstream>

TCPSocketServer::TCPSocketServer(const int &port, std::shared_ptr<Database> database) : port(port), database(database), running(false)
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
    char buffer[MAX_DATA_TRANSFER_SIZE];
    int bytesRead;
    while ((bytesRead = read(clientSocket, buffer, MAX_DATA_TRANSFER_SIZE)) > 0)
    {
        buffer[bytesRead] = '\0';
        std::cout << "Received message: " << buffer << std::endl;

        std::stringstream receivedMessageStream(buffer);
        std::string requestType;

        std::getline(receivedMessageStream, requestType, ',');

        if (requestType == "CLOSE_CONNECTION")
        {
            close(clientSocket);
            return;
        }

        if (requestType == "LOGIN_USER")
        {
            std::string user_id;
            std::string password;

            std::getline(receivedMessageStream, user_id, ',');
            std::getline(receivedMessageStream, password, ',');

            std::vector<std::vector<std::string>> databaseResponse = database->fetchRows("SELECT * FROM User WHERE user_id = " + user_id + " AND password = '" + password + "'");

            std::string response = "FAILURE";

            if (databaseResponse.size() == 1)
            {
                response = "SUCCESS";
                for (std::string col : databaseResponse[0])
                {
                    response += "," + col;
                }
            }

            write(clientSocket, response.c_str(), response.size());
        }

        if (requestType == "ADD_USER")
        {
            std::string userId;
            std::string name;
            std::string password;
            std::string roleStr;
            std::string notification_number;

            std::getline(receivedMessageStream, userId, ',');
            std::getline(receivedMessageStream, name, ',');
            std::getline(receivedMessageStream, password, ',');
            std::getline(receivedMessageStream, roleStr, ',');
            std::getline(receivedMessageStream, notification_number, ',');

            std::string response = "FAILURE";
            try
            {
                database->executeQuery("INSERT INTO User(name, password, role, notification_number) values('" + name + "','" + password + "','" + roleStr + "'," + notification_number + ")");

                std::vector<std::vector<std::string>> queryResult = database->fetchRows("SELECT user_id FROM User WHERE name = '" + name + "'");
                std::string newUserId = queryResult[0][0];
                response = "SUCCESS," + newUserId;
            }
            catch (const DatabaseException &e)
            {
                std::cout << e.what() << std::endl;
            }

            write(clientSocket, response.c_str(), response.size());
        }

        if (requestType == "DELETE_USER")
        {
            std::string userId;
            std::getline(receivedMessageStream, userId, ',');

            std::string response = "FAILURE";
            try
            {
                if (database->fetchRows("SELECT * FROM User WHERE user_id = " + userId).size() > 0)
                {
                    database->executeQuery("DELETE FROM User WHERE user_id = " + userId);
                    response = "SUCCESS";
                }
            }
            catch (const DatabaseException &e)
            {
                std::cout << e.what() << std::endl;
            }

            write(clientSocket, response.c_str(), response.size());
        }

        if (requestType == "GET_ALL_USERS")
        {
            std::string response = "FAILURE";
            try
            {
                std::vector<std::vector<std::string>> queryResult = database->fetchRows("SELECT * FROM User");

                response = "SUCCESS";

                int rowCount = queryResult.size();

                response += "," + std::to_string(rowCount);

                for (std::vector<std::string> row : queryResult)
                {
                    for (std::string col : row)
                    {
                        response += "," + col;
                    }
                }
            }
            catch (const DatabaseException &e)
            {
                std::cout << e.what() << std::endl;
            }

            write(clientSocket, response.c_str(), response.size());
        }

        if (requestType == "ADD_MENU_ITEM")
        {
            std::string itemId;
            std::string name;
            std::string price;
            std::string description;
            std::string category;
            std::string availability;
            std::string likes;
            std::string dislikes;

            std::getline(receivedMessageStream, itemId, ',');
            std::getline(receivedMessageStream, name, ',');
            std::getline(receivedMessageStream, price, ',');
            std::getline(receivedMessageStream, description, ',');
            std::getline(receivedMessageStream, category, ',');
            std::getline(receivedMessageStream, availability, ',');

            availability = (availability == "0") ? "false" : "true";

            std::string response = "FAILURE";
            try
            {
                database->executeQuery("INSERT INTO Menu_Item"
                                       "(name, price, description, category, availability)"
                                       " VALUES ('" +
                                       name + "', " + price + ", '" + description +
                                       "', '" + category + "', " + availability + ")");

                std::vector<std::vector<std::string>> queryResult = database->fetchRows("SELECT item_id FROM Menu_Item WHERE name = '" + name + "'");
                std::string newItemId = queryResult[0][0];
                response = "SUCCESS," + newItemId;
            }
            catch (const DatabaseException &e)
            {
                std::cout << e.what() << std::endl;
            }

            write(clientSocket, response.c_str(), response.size());
        }

        if (requestType == "DELETE_ITEM")
        {
            std::string itemId;
            std::getline(receivedMessageStream, itemId, ',');

            std::string response = "FAILURE";
            try
            {
                if (database->fetchRows("SELECT * FROM Menu_Item WHERE item_id = " + itemId).size() > 0)
                {
                    database->executeQuery("DELETE FROM Menu_Item WHERE item_id = " + itemId);
                    response = "SUCCESS";
                }
            }
            catch (const DatabaseException &e)
            {
                std::cout << e.what() << std::endl;
            }

            write(clientSocket, response.c_str(), response.size());
        }

        if (requestType == "GET_ALL_MENU_ITEMS")
        {
            std::string response = "SUCCESS";
            try
            {
                std::vector<std::vector<std::string>> queryResult = database->fetchRows("SELECT * FROM Menu_Item");

                int rowCount = queryResult.size();
                response += "," + std::to_string(rowCount);

                for (std::vector<std::string> row : queryResult)
                {
                    for (std::string col : row)
                    {
                        response += "," + col;
                    }

                    std::vector<std::vector<std::string>> sentiments = database->fetchRows("SELECT * FROM Menu_Item_Sentiment WHERE item_id = " + row[0]);
                    rowCount = sentiments.size();
                    response += "," + std::to_string(rowCount);
                    for (std::vector<std::string> sentiment : sentiments)
                    {
                        response += "," + sentiment[1];
                    }
                }
            }
            catch (const DatabaseException &e)
            {
                response = "FAILURE";
                std::cout << e.what() << std::endl;
            }
            std::cout << response << std::endl;
            write(clientSocket, response.c_str(), response.size());
        }
    }
}
