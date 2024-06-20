#include "exception.h"
#include "mysqlDatabase.h"
#include "simpleSentimentAnalyzer.h"
#include "tcpSocketServer.h"

#include <iostream>

void startServer()
{
    try
    {
        std::shared_ptr<SimpleSentimentAnalyzer> analyzer = std::make_shared<SimpleSentimentAnalyzer>("./assets/positiveSentiments.csv", "./assets/negativeSentiments.csv");
        MySQLDatabase::createInstance("tcp://127.0.0.1:3306", "shrinath", "shri@123", "cafeteria");

        TCPSocketServer server(8080, MySQLDatabase::getInstance(), analyzer);
        server.connect();
    }
    catch (const CustomException &e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
}

std::string getCurrentDate()
{
    time_t t = time(0);
    tm *currentDateTime = localtime(&t);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", currentDateTime);

    return std::string(buffer);
}
