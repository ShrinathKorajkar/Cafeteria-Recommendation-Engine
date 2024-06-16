#include "tcpSocketServer.h"
#include "exception.h"
#include "mysqlDatabase.h"

#include <iostream>
#include <thread>

void startServer()
{
    try
    {
        MySQLDatabase::createInstance("tcp://127.0.0.1:3306", "shrinath", "shri@123", "cafeteria");
        TCPSocketServer server(8080, MySQLDatabase::getInstance());
        server.connect();
    }
    catch (const CustomException &e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
}

int main()
{
    std::thread serverThread(startServer);
    serverThread.join();

    return 0;
}
