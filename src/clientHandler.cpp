#include "clientHandler.h"
#include "exception.h"
#include "mysqlDatabase.h"
#include "utility.h"

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <unordered_map>

ClientHandler::ClientHandler(std::shared_ptr<Database> database, std::shared_ptr<SentimentAnalyzer> analyzer, const int &clientSocket)
    : database(database), analyzer(analyzer), clientSocket(clientSocket) {}

void ClientHandler::start()
{
    char buffer[MAX_DATA_TRANSFER_SIZE];
    int bytesRead;
    while ((bytesRead = read(clientSocket, buffer, MAX_DATA_TRANSFER_SIZE)) > 0)
    {
        buffer[bytesRead] = '\0';
        std::string request(buffer);
        handleRequest(request);
    }
    close(clientSocket);
}

void ClientHandler::handleRequest(const std::string &request)
{
    std::stringstream receivedMessageStream(request);
    std::string requestType;
    std::getline(receivedMessageStream, requestType, getDelimiterChar());
    RequestCode requestCode = stringToRequestCode(requestType);

    static const std::unordered_map<RequestCode, void (ClientHandler::*)(std::stringstream &)> handlers = {
        {RequestCode::LOGIN_USER, &ClientHandler::handleLoginUser},
        {RequestCode::ADD_USER, &ClientHandler::handleAddUser},
        {RequestCode::DELETE_USER, &ClientHandler::handleDeleteUser},
        {RequestCode::ADD_MENU_ITEM, &ClientHandler::handleAddMenuItem},
        {RequestCode::DELETE_MENU_ITEM, &ClientHandler::handleDeleteItem},
        {RequestCode::GET_ALL_USERS, &ClientHandler::handleGetAllUsers},
        {RequestCode::GET_ALL_MENU_ITEMS, &ClientHandler::handleGetAllMenuItems},
        {RequestCode::GET_NOTIFICATIONS, &ClientHandler::handleGetPendingNotifications},
        {RequestCode::ROLL_OUT_DAILY_MENU, &ClientHandler::handleRollOutDailyMenu},
        {RequestCode::GET_RESPONSE_ORDERS, &ClientHandler::handleGetResponseOrders},
        {RequestCode::GET_RECOMMENDED_MENU, &ClientHandler::handleGetRecommendedMenu},
        {RequestCode::GENERATE_REPORT, &ClientHandler::handleGenerateReport},
        {RequestCode::GET_DAILY_MENU, &ClientHandler::handleGetDailyMenu},
        {RequestCode::ORDER_FOOD, &ClientHandler::handleOrderFood},
        {RequestCode::GET_TODAYS_ORDER, &ClientHandler::handleGetTodaysOrder},
        {RequestCode::LIKE_DISLIKE, &ClientHandler::handleLikeDislike},
        {RequestCode::GIVE_FEEDBACK, &ClientHandler::handleGiveFeedback},
        {RequestCode::CLOSE_CONNECTION, &ClientHandler::handleCloseConnection},
    };

    auto handler = handlers.find(requestCode);
    if (handler != handlers.end())
    {
        (this->*(handler->second))(receivedMessageStream);
    }
    else
    {
        std::cerr << "Unknown request type: " << requestType << std::endl;
    }
}

void ClientHandler::handleCloseConnection(std::stringstream &)
{
    close(clientSocket);
}

void ClientHandler::handleLoginUser(std::stringstream &receivedMessageStream)
{
    std::string user_id, password;
    std::getline(receivedMessageStream, user_id, getDelimiterChar());
    std::getline(receivedMessageStream, password, getDelimiterChar());

    auto databaseResponse = database->fetchRows("SELECT * FROM User WHERE user_id = " + user_id + " AND password = '" + password + "'");
    std::string response = (databaseResponse.size() == 1) ? "SUCCESS" : "FAILURE";

    if (response == "SUCCESS")
    {
        for (const auto &col : databaseResponse[0])
        {
            response += getDelimiterString() + col;
        }
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleAddUser(std::stringstream &receivedMessageStream)
{
    std::string userId, name, password, roleStr, notification_number;
    std::getline(receivedMessageStream, userId, getDelimiterChar());
    std::getline(receivedMessageStream, name, getDelimiterChar());
    std::getline(receivedMessageStream, password, getDelimiterChar());
    std::getline(receivedMessageStream, roleStr, getDelimiterChar());
    std::getline(receivedMessageStream, notification_number, getDelimiterChar());

    std::string response = "FAILURE";
    try
    {
        database->executeQuery("INSERT INTO User(name, password, role, notification_number) values('" + name + "," + password + "," + roleStr + "'," + notification_number + ")");
        auto queryResult = database->fetchRows("SELECT user_id FROM User WHERE name = '" + name + "'");
        response = "SUCCESS," + queryResult[0][0];
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleDeleteUser(std::stringstream &receivedMessageStream)
{
    std::string userId;
    std::getline(receivedMessageStream, userId, getDelimiterChar());

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

void ClientHandler::handleGetAllUsers(std::stringstream &)
{
    std::string response = "FAILURE";
    try
    {
        auto queryResult = database->fetchRows("SELECT * FROM User");
        response = "SUCCESS," + std::to_string(queryResult.size());

        for (const auto &row : queryResult)
        {
            for (const auto &col : row)
            {
                response += getDelimiterString() + col;
            }
        }
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleAddMenuItem(std::stringstream &receivedMessageStream)
{
    std::string name, price, description, category, availability;
    std::getline(receivedMessageStream, name, getDelimiterChar());
    std::getline(receivedMessageStream, price, getDelimiterChar());
    std::getline(receivedMessageStream, description, getDelimiterChar());
    std::getline(receivedMessageStream, category, getDelimiterChar());
    std::getline(receivedMessageStream, availability, getDelimiterChar());

    availability = (availability == "0") ? "false" : "true";

    std::string response = "FAILURE";
    try
    {
        database->executeQuery("INSERT INTO Menu_Item(name, price, description, category, availability) VALUES ('" + name + "', " + price + ", '" + description + "', '" + category + "', " + availability + ")");
        auto queryResult = database->fetchRows("SELECT item_id FROM Menu_Item WHERE name = '" + name + "'");
        response = "SUCCESS," + queryResult[0][0];
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleDeleteItem(std::stringstream &receivedMessageStream)
{
    std::string itemId;
    std::getline(receivedMessageStream, itemId, getDelimiterChar());

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

void ClientHandler::handleGetAllMenuItems(std::stringstream &)
{
    std::string response = "SUCCESS";
    try
    {
        std::string query = "SELECT * FROM Menu_Item ORDER BY recommend_rating DESC";

        std::vector<std::vector<std::string>> queryResult = database->fetchRows(query);

        int rowCount = queryResult.size();
        response += getDelimiterString() + std::to_string(rowCount);

        for (std::vector<std::string> row : queryResult)
        {
            for (std::string col : row)
            {
                response += getDelimiterString() + col;
            }

            std::vector<std::vector<std::string>> sentiments = database->fetchRows("SELECT * FROM Menu_Item_Sentiment WHERE item_id = " + row[0]);
            int sentimentCount = sentiments.size();
            response += getDelimiterString() + std::to_string(sentimentCount);
            for (std::vector<std::string> sentiment : sentiments)
            {
                response += getDelimiterString() + sentiment[1];
            }

            std::vector<std::vector<std::string>> comments = database->fetchRows("SELECT u.name, c.comment, c.comment_date "
                                                                                 "FROM Comment c JOIN User u ON c.user_id = u.user_id "
                                                                                 "WHERE c.item_id = " +
                                                                                 row[0] + " ORDER BY Comment_date DESC");
            int commentCount = comments.size();
            response += getDelimiterString() + std::to_string(commentCount);
            for (std::vector<std::string> comment : comments)
            {
                response += getDelimiterString() + comment[0] + getDelimiterString() + comment[1] + getDelimiterString() + comment[2];
            }
        }
    }
    catch (const DatabaseException &e)
    {
        response = "FAILURE";
        std::cout << e.what() << std::endl;
    }
    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleGetRecommendedMenu(std::stringstream &receivedMessageStream)
{
    std::string response = "SUCCESS";
    try
    {
        std::string query = "SELECT * FROM Menu_Item WHERE availability = true ORDER BY category, recommend_rating DESC";

        std::vector<std::vector<std::string>> queryResult = database->fetchRows(query);

        int rowCount = queryResult.size();
        response += getDelimiterString() + std::to_string(rowCount);

        for (std::vector<std::string> row : queryResult)
        {
            for (std::string col : row)
            {
                response += getDelimiterString() + col;
            }

            std::vector<std::vector<std::string>> sentiments = database->fetchRows("SELECT * FROM Menu_Item_Sentiment WHERE item_id = " + row[0]);
            int sentimentCount = sentiments.size();
            response += getDelimiterString() + std::to_string(sentimentCount);
            for (std::vector<std::string> sentiment : sentiments)
            {
                response += getDelimiterString() + sentiment[1];
            }

            std::vector<std::vector<std::string>> comments = database->fetchRows("SELECT u.name, c.comment, c.comment_date "
                                                                                 "FROM Comment c JOIN User u ON c.user_id = u.user_id "
                                                                                 "WHERE c.item_id = " +
                                                                                 row[0] + " ORDER BY Comment_date DESC");
            int commentCount = comments.size();
            response += getDelimiterString() + std::to_string(commentCount);
            for (std::vector<std::string> comment : comments)
            {
                response += getDelimiterString() + comment[0] + getDelimiterString() + comment[1] + getDelimiterString() + comment[2];
            }
        }
    }
    catch (const DatabaseException &e)
    {
        response = "FAILURE";
        std::cout << e.what() << std::endl;
    }
    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleGetPendingNotifications(std::stringstream &receivedMessageStream)
{
    std::string response = "FAILURE";
    std::string userId;
    std::getline(receivedMessageStream, userId, getDelimiterChar());

    try
    {
        auto notificationData = database->fetchRows("SELECT notification FROM Notifications WHERE user_id = " + userId + " AND status = 'pending'");
        response = "SUCCESS," + std::to_string(notificationData.size());

        for (const auto &notification : notificationData)
        {
            response += getDelimiterString() + notification[0];
        }
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleRollOutDailyMenu(std::stringstream &)
{
    std::string response = "FAILURE";
    try
    {
        database->executeQuery("UPDATE Menu_Item SET daily = true");
        response = "SUCCESS";
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleGenerateReport(std::stringstream &)
{
    std::string response = "FAILURE";
    try
    {
        auto reportData = database->fetchRows("SELECT * FROM Report");
        response = "SUCCESS," + std::to_string(reportData.size());

        for (const auto &row : reportData)
        {
            for (const auto &col : row)
            {
                response += getDelimiterString() + col;
            }
        }
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleGetResponseOrders(std::stringstream &)
{
    std::string response = "FAILURE";
    try
    {
        auto orderData = database->fetchRows("SELECT * FROM Orders WHERE status = 'pending'");
        response = "SUCCESS," + std::to_string(orderData.size());

        for (const auto &row : orderData)
        {
            for (const auto &col : row)
            {
                response += getDelimiterString() + col;
            }
        }
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleGetDailyMenu(std::stringstream &)
{
    std::string response = "FAILURE";
    try
    {
        auto menuData = database->fetchRows("SELECT * FROM Menu_Item WHERE daily = true");
        response = "SUCCESS," + std::to_string(menuData.size());

        for (const auto &row : menuData)
        {
            for (const auto &col : row)
            {
                response += getDelimiterString() + col;
            }
        }
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleOrderFood(std::stringstream &receivedMessageStream)
{
    std::string userId, itemId;
    std::getline(receivedMessageStream, userId, getDelimiterChar());
    std::getline(receivedMessageStream, itemId, getDelimiterChar());

    std::string response = "FAILURE";
    try
    {
        database->executeQuery("INSERT INTO Orders(user_id, item_id, status) VALUES (" + userId + ", " + itemId + ", 'pending')");
        response = "SUCCESS";
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleGetTodaysOrder(std::stringstream &receivedMessageStream)
{
    std::string userId;
    std::getline(receivedMessageStream, userId, getDelimiterChar());

    std::string response = "FAILURE";
    try
    {
        auto orderData = database->fetchRows("SELECT * FROM Orders WHERE user_id = " + userId + " AND date = CURDATE()");
        response = "SUCCESS," + std::to_string(orderData.size());

        for (const auto &row : orderData)
        {
            for (const auto &col : row)
            {
                response += getDelimiterString() + col;
            }
        }
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleLikeDislike(std::stringstream &receivedMessageStream)
{
    std::string userId, itemId, likeDislike;
    std::getline(receivedMessageStream, userId, getDelimiterChar());
    std::getline(receivedMessageStream, itemId, getDelimiterChar());
    std::getline(receivedMessageStream, likeDislike, getDelimiterChar());

    std::string response = "FAILURE";
    try
    {
        database->executeQuery("INSERT INTO Feedback(user_id, item_id, like_dislike) VALUES (" + userId + ", " + itemId + ", " + likeDislike + ")");
        response = "SUCCESS";
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleGiveFeedback(std::stringstream &receivedMessageStream)
{
    std::string userId, itemId, feedback;
    std::getline(receivedMessageStream, userId, getDelimiterChar());
    std::getline(receivedMessageStream, itemId, getDelimiterChar());
    std::getline(receivedMessageStream, feedback, getDelimiterChar());

    std::string response = "FAILURE";
    try
    {
        database->executeQuery("INSERT INTO Feedback(user_id, item_id, feedback) VALUES (" + userId + ", " + itemId + ", '" + feedback + "')");
        response = "SUCCESS";
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}
