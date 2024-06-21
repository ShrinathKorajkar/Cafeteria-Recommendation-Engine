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

        std::cout << "received : " << request << std::endl;
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
    std::string userId, password;
    std::getline(receivedMessageStream, userId, getDelimiterChar());
    std::getline(receivedMessageStream, password, getDelimiterChar());

    std::string response = "FAILURE";
    try
    {
        std::string query = "SELECT * FROM User WHERE user_id = " + userId + " AND password = '" + password + "'";
        auto queryResult = database->fetchRows(query);
        if (queryResult.size() == 1)
        {
            response = "SUCCESS";
            for (std::string col : queryResult[0])
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
        database->executeQuery("INSERT INTO User(name, password, role, notification_number) values('" + name + "getDelimiterChar()" + password + "getDelimiterChar()" + roleStr + "'," + notification_number + ")");
        auto queryResult = database->fetchRows("SELECT user_id FROM User WHERE name = '" + name + "'");
        std::string userIdCol = queryResult[0][0];
        response = "SUCCESS" + getDelimiterString() + userIdCol;
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
        auto query = "SELECT user_id FROM User WHERE user_id = " + userId;
        if (database->fetchRows(query).size() > 0)
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
        response = "SUCCESS" + getDelimiterString() + std::to_string(queryResult.size());

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
    std::string itemId, name, price, description, category, availability;
    std::getline(receivedMessageStream, itemId, getDelimiterChar());
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
        response = "SUCCESS" + getDelimiterString() + queryResult[0][0];
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
    std::string userId;
    std::getline(receivedMessageStream, userId, getDelimiterChar());

    std::string notificationNumber;
    std::getline(receivedMessageStream, notificationNumber, getDelimiterChar());

    std::string response = "FAILURE";
    try
    {
        std::vector<std::vector<std::string>> queryResult = database->fetchRows("SELECT message, notification_id FROM Notification "
                                                                                "WHERE notification_date = CURDATE() AND notification_id > " +
                                                                                notificationNumber + " ORDER BY notification_id ASC");

        int rowCount = queryResult.size();
        if (rowCount > 0)
        {
            std::string maxNotificationNumber = queryResult[queryResult.size() - 1][1];
            database->executeQuery("UPDATE User SET notification_number = " + maxNotificationNumber + " WHERE user_id = " + userId);

            response = "SUCCESS," + maxNotificationNumber;
            for (std::vector<std::string> row : queryResult)
            {
                response += getDelimiterString() + row[0];
            }
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
    std::string menuSizeStr;
    std::getline(receivedMessageStream, menuSizeStr, getDelimiterChar());
    int menuSize = std::stoi(menuSizeStr);

    std::string query = "INSERT INTO Daily_Menu(item_id) VALUES";
    std::string itemId;
    for (int i = 0; i < menuSize - 1; i++)
    {
        std::getline(receivedMessageStream, itemId, getDelimiterChar());
        query += "(" + itemId + "),";
    }
    std::getline(receivedMessageStream, itemId, getDelimiterChar());
    query += "(" + itemId + ");";

    std::string response = "FAILURE";

    try
    {
        database->executeQuery(query);
        std::string notificationMessage = "Chef has rolled out Menu for " + getCurrentDate() + ". Checkout Now";
        database->executeQuery("INSERT INTO Notification(message) VALUES ('" + notificationMessage + "')");
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
    std::string month;
    std::string year;

    std::getline(receivedMessageStream, month, getDelimiterChar());
    std::getline(receivedMessageStream, year, getDelimiterChar());

    std::string response = "FAILURE";
    try
    {
        std::string query = "SELECT mi.name, COUNT(oi.item_id) AS total_orders "
                            "FROM Menu_Item mi "
                            "JOIN Order_Item oi ON mi.item_id = oi.item_id "
                            "JOIN User_Order uo ON oi.order_id = uo.order_id "
                            "WHERE YEAR(uo.order_date) = " +
                            year + " AND MONTH(uo.order_date) = " + month +
                            " GROUP BY mi.item_id, mi.name, mi.category "
                            "ORDER BY total_orders DESC";

        std::vector<std::vector<std::string>> report = database->fetchRows(query);
        response = "SUCCESS";

        for (const auto &reportRow : report)
        {
            for (const auto &reportCol : reportRow)
            {
                response += getDelimiterString() + reportCol;
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
        std::string query = "SELECT mi.name, COUNT(oi.item_id) as orderCount "
                            "FROM Menu_Item mi "
                            "JOIN Order_Item oi ON mi.item_id = oi.item_id "
                            "JOIN User_Order uo ON oi.order_id = uo.order_id "
                            "WHERE uo.order_date = CURDATE() "
                            "GROUP BY mi.name ORDER BY orderCount DESC";

        std::vector<std::vector<std::string>> orders = database->fetchRows(query);

        if (orders.size() != 0)
        {
            response = "SUCCESS";

            for (const auto &ordersRow : orders)
            {
                for (const auto &ordersCol : ordersRow)
                {
                    response += getDelimiterString() + ordersCol;
                }
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
    std::string response = "SUCCESS";
    try
    {
        std::string query = "SELECT mi.* FROM Menu_Item mi "
                            "INNER JOIN Daily_Menu dm ON mi.item_id = dm.item_id "
                            "WHERE dm.menu_date = CURDATE()";

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
        std::cout << e.what() << std::endl;
    }
    write(clientSocket, response.c_str(), response.size());
}

void ClientHandler::handleOrderFood(std::stringstream &receivedMessageStream)
{
    std::string userId;
    std::getline(receivedMessageStream, userId, getDelimiterChar());
    std::string response = "FAILURE";
    try
    {
        std::string query = "INSERT INTO User_Order(user_id) VALUES(" + userId + ")";
        database->executeQuery(query);

        query = "SELECT order_id FROM User_Order WHERE user_id = " + userId + " ORDER BY order_id DESC LIMIT 1";
        std::vector<std::vector<std::string>> orderId = database->fetchRows(query);
        std::string currentOrderId = orderId[0][0];

        query = "INSERT INTO Order_Item(order_id, item_id) VALUES";

        std::string itemId;
        std::getline(receivedMessageStream, itemId, getDelimiterChar());
        query += "(" + currentOrderId + getDelimiterString() + itemId + ")";

        while (std::getline(receivedMessageStream, itemId, getDelimiterChar()))
        {
            query += ",(" + currentOrderId + getDelimiterString() + itemId + ")";
        }

        database->executeQuery(query);
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
        std::string query = "SELECT oi.item_id, mi.name "
                            "FROM User_Order uo "
                            "JOIN Order_Item oi ON uo.order_id = oi.order_id "
                            "JOIN Menu_Item mi ON oi.item_id = mi.item_id "
                            "WHERE uo.user_id = " +
                            userId + " AND uo.order_date = CURDATE() - INTERVAL 1 DAY";

        std::vector<std::vector<std::string>> orders = database->fetchRows(query);

        response = "SUCCESS";
        for (const auto &order : orders)
        {
            for (const auto &item : order)
            {
                response += getDelimiterString() + item;
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
    std::string itemId;
    std::getline(receivedMessageStream, itemId, getDelimiterChar());

    std::string likeStatus;
    std::getline(receivedMessageStream, likeStatus, getDelimiterChar());

    std::string response = "FAILURE";
    try
    {
        std::string query;
        if (likeStatus == "like")
        {
            query = "UPDATE Menu_Item SET likes = likes + 1 WHERE item_id = " + itemId;
        }
        else
        {
            query = "UPDATE Menu_Item SET dislikes = dislikes + 1 WHERE item_id = " + itemId;
        }

        database->executeQuery(query);
        updateRecommendRating(itemId);

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
    std::string itemId;
    std::getline(receivedMessageStream, itemId, getDelimiterChar());

    std::string userId;
    std::getline(receivedMessageStream, userId, getDelimiterChar());

    std::string comment;
    std::getline(receivedMessageStream, comment, getDelimiterChar());

    std::string response = "FAILURE";
    try
    {
        std::vector<std::string> postivieSentiments = analyzer->getPositiveSentiments(comment);
        std::vector<std::string> negativeSentiments = analyzer->getNegativeSentiments(comment);

        std::string query = "INSERT INTO Comment(user_id, item_id, comment) VALUES(" + userId + getDelimiterString() + itemId + ",'" + comment + "')";
        database->executeQuery(query);

        int positiveSentimentSize = postivieSentiments.size();
        int negativeSentimentSize = negativeSentiments.size();
        int totalSentimentSize = positiveSentimentSize + negativeSentimentSize;
        if (totalSentimentSize > 0)
        {
            query = "INSERT INTO Menu_Item_Sentiment(item_id, sentiment, type) VALUES";
            int i = 0;
            int j = 0;

            if (postivieSentiments.size() > 0)
            {
                query += "(" + itemId + ",'" + postivieSentiments[i++] + "getDelimiterChar()positive')";
            }
            else
            {
                query += "(" + itemId + ",'" + negativeSentiments[j++] + "getDelimiterChar()negative')";
            }

            for (; i < postivieSentiments.size(); i++)
            {
                query += ",(" + itemId + ",'" + postivieSentiments[i] + "getDelimiterChar()positive')";
            }
            for (; i < postivieSentiments.size(); i++)
            {
                query += ",(" + itemId + ",'" + negativeSentiments[j] + "getDelimiterChar()negative')";
            }

            updateRecommendRating(itemId);
            database->executeQuery(query);
        }

        response = "SUCCESS";
    }
    catch (const DatabaseException &e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }

    write(clientSocket, response.c_str(), response.size());
}
