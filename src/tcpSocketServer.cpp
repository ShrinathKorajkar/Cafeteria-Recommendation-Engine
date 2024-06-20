#include "tcpSocketServer.h"
#include "exception.h"
#include "mysqlDatabase.h"

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
    char buffer[MAX_DATA_TRANSFER_SIZE];
    int bytesRead;
    while ((bytesRead = read(clientSocket, buffer, MAX_DATA_TRANSFER_SIZE)) > 0)
    {
        buffer[bytesRead] = '\0';

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

                std::string notificationMessage = "New Menu Item Added : " + name + ". Checkout Menu";
                database->executeQuery("INSERT INTO Notification(message) VALUES ('" + notificationMessage + "')");

                std::vector<std::vector<std::string>>
                    queryResult = database->fetchRows("SELECT item_id FROM Menu_Item WHERE name = '" + name + "'");
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
                response = "FAILURE";
                std::cout << e.what() << std::endl;
            }

            write(clientSocket, response.c_str(), response.size());
        }

        if (requestType == "GET_ALL_MENU_ITEMS" || requestType == "GET_RECOMMENDED_MENU")
        {
            std::string response = "SUCCESS";
            try
            {
                std::string query;

                if (requestType == "GET_RECOMMENDED_ITEM")
                {
                    query = "SELECT * FROM Menu_Item WHERE availability = true ORDER BY category, recommend_rating DESC";
                }
                else
                {
                    query = "SELECT * FROM Menu_Item ORDER BY recommend_rating DESC";
                }

                std::vector<std::vector<std::string>> queryResult = database->fetchRows(query);

                int rowCount = queryResult.size();
                response += "," + std::to_string(rowCount);

                for (std::vector<std::string> row : queryResult)
                {
                    for (std::string col : row)
                    {
                        response += "," + col;
                    }

                    std::vector<std::vector<std::string>> sentiments = database->fetchRows("SELECT * FROM Menu_Item_Sentiment WHERE item_id = " + row[0]);
                    int sentimentCount = sentiments.size();
                    response += "," + std::to_string(sentimentCount);
                    for (std::vector<std::string> sentiment : sentiments)
                    {
                        response += "," + sentiment[1];
                    }

                    std::vector<std::vector<std::string>> comments = database->fetchRows("SELECT u.name, c.comment, c.comment_date "
                                                                                         "FROM Comment c JOIN User u ON c.user_id = u.user_id "
                                                                                         "WHERE c.item_id = " +
                                                                                         row[0] + " ORDER BY Comment_date DESC");
                    int commentCount = comments.size();
                    response += "," + std::to_string(commentCount);
                    for (std::vector<std::string> comment : comments)
                    {
                        response += "," + comment[0] + "," + comment[1] + "," + comment[2];
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

        if (requestType == "GET_PENDING_NOTIFICATIONS")
        {
            std::string userId;
            std::getline(receivedMessageStream, userId, ',');

            std::string notificationNumber;
            std::getline(receivedMessageStream, notificationNumber, ',');

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
                        response += "," + row[0];
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

        if (requestType == "ROLL_OUT_DAILY_MENU")
        {
            std::string menuSizeStr;
            std::getline(receivedMessageStream, menuSizeStr, ',');
            int menuSize = std::stoi(menuSizeStr);

            std::string query = "INSERT INTO Daily_Menu(item_id) VALUES";
            std::string itemId;
            for (int i = 0; i < menuSize - 1; i++)
            {
                std::getline(receivedMessageStream, itemId, ',');
                query += "(" + itemId + "),";
            }
            std::getline(receivedMessageStream, itemId, ',');
            query += "(" + itemId + ");";

            std::string response = "SUCCESS";

            try
            {
                database->executeQuery(query);
                std::string notificationMessage = "Chef has rolled out Menu for " + getCurrentDate() + ". Checkout Now";
                database->executeQuery("INSERT INTO Notification(message) VALUES ('" + notificationMessage + "')");
            }
            catch (const DatabaseException &e)
            {
                response = "FAILURE";
                std::cout << e.what() << std::endl;
            }

            write(clientSocket, response.c_str(), response.size());
        }

        if (requestType == "GENERATE_REPORT")
        {
            std::string month;
            std::string year;

            std::getline(receivedMessageStream, month, ',');
            std::getline(receivedMessageStream, year, ',');

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
                        response += "," + reportCol;
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

        if (requestType == "GET_RESPONSE_ORDERS")
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
                            response += "," + ordersCol;
                        }
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

        if (requestType == "GET_DAILY_MENU")
        {
            std::string response = "SUCCESS";
            try
            {
                std::string query = "SELECT mi.* FROM Menu_Item mi "
                                    "INNER JOIN Daily_Menu dm ON mi.item_id = dm.item_id "
                                    "WHERE dm.menu_date = CURDATE()";

                std::vector<std::vector<std::string>> queryResult = database->fetchRows(query);

                int rowCount = queryResult.size();
                response += "," + std::to_string(rowCount);

                for (std::vector<std::string> row : queryResult)
                {
                    for (std::string col : row)
                    {
                        response += "," + col;
                    }

                    std::vector<std::vector<std::string>> sentiments = database->fetchRows("SELECT * FROM Menu_Item_Sentiment WHERE item_id = " + row[0]);
                    int sentimentCount = sentiments.size();
                    response += "," + std::to_string(sentimentCount);
                    for (std::vector<std::string> sentiment : sentiments)
                    {
                        response += "," + sentiment[1];
                    }

                    std::vector<std::vector<std::string>> comments = database->fetchRows("SELECT u.name, c.comment, c.comment_date "
                                                                                         "FROM Comment c JOIN User u ON c.user_id = u.user_id "
                                                                                         "WHERE c.item_id = " +
                                                                                         row[0] + " ORDER BY Comment_date DESC");
                    int commentCount = comments.size();
                    response += "," + std::to_string(commentCount);
                    for (std::vector<std::string> comment : comments)
                    {
                        response += "," + comment[0] + "," + comment[1] + "," + comment[2];
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

        if (requestType == "ORDER_FOOD")
        {
            std::string userId;
            std::getline(receivedMessageStream, userId, ',');
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
                std::getline(receivedMessageStream, itemId, ',');
                query += "(" + currentOrderId + "," + itemId + ")";

                while (std::getline(receivedMessageStream, itemId, ','))
                {
                    query += ",(" + currentOrderId + "," + itemId + ")";
                }

                database->executeQuery(query);
                response = "SUCCESS";
            }
            catch (const DatabaseException &e)
            {
                response = "FAILURE";
                std::cout << e.what() << std::endl;
            }

            write(clientSocket, response.c_str(), response.size());
        }

        if (requestType == "GET_TODAYS_ORDER")
        {
            std::string userId;
            std::getline(receivedMessageStream, userId, ',');

            std::string response = "SUCCESS";
            try
            {
                std::string query = "SELECT oi.item_id, mi.name "
                                    "FROM User_Order uo "
                                    "JOIN Order_Item oi ON uo.order_id = oi.order_id "
                                    "JOIN Menu_Item mi ON oi.item_id = mi.item_id "
                                    "WHERE uo.user_id = " +
                                    userId + " AND uo.order_date = CURDATE() - INTERVAL 1 DAY";

                std::vector<std::vector<std::string>> orders = database->fetchRows(query);

                for (const auto &order : orders)
                {
                    for (const auto &item : order)
                    {
                        response += "," + item;
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

        if (requestType == "LIKE_DISLIKE")
        {
            std::string itemId;
            std::getline(receivedMessageStream, itemId, ',');

            std::string likeStatus;
            std::getline(receivedMessageStream, likeStatus, ',');

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
                response = "FAILURE";
                std::cout << e.what() << std::endl;
            }
            write(clientSocket, response.c_str(), response.size());
        }

        if (requestType == "GIVE_FEEDBACK")
        {
            std::string itemId;
            std::getline(receivedMessageStream, itemId, ',');

            std::string userId;
            std::getline(receivedMessageStream, userId, ',');

            std::string comment;
            std::getline(receivedMessageStream, comment, ',');

            std::string response = "FAILURE";
            try
            {
                std::vector<std::string> postivieSentiments = analyzer->getPositiveSentiments(comment);
                std::vector<std::string> negativeSentiments = analyzer->getNegativeSentiments(comment);

                std::string query = "INSERT INTO Comment(user_id, item_id, comment) VALUES(" + userId + "," + itemId + ",'" + comment + "')";
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
                        query += "(" + itemId + ",'" + postivieSentiments[i++] + "','positive')";
                    }
                    else
                    {
                        query += "(" + itemId + ",'" + negativeSentiments[j++] + "','negative')";
                    }

                    for (; i < postivieSentiments.size(); i++)
                    {
                        query += ",(" + itemId + ",'" + postivieSentiments[i] + "','positive')";
                    }
                    for (; i < postivieSentiments.size(); i++)
                    {
                        query += ",(" + itemId + ",'" + negativeSentiments[j] + "','negative')";
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
    }
}

std::string TCPSocketServer::getCurrentDate()
{
    time_t t = time(0);
    tm *currentDateTime = localtime(&t);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", currentDateTime);

    return std::string(buffer);
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