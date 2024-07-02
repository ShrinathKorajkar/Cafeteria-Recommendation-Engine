#pragma once

#include "exception.h"
#include "mysqlDatabase.h"
#include "sentimentAnalyzer.h"

#include <unistd.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

class ClientHandler
{
    std::shared_ptr<Database> database;
    std::shared_ptr<SentimentAnalyzer> analyzer;
    int clientSocket;
    int MAX_DATA_TRANSFER_SIZE = 32768;

public:
    ClientHandler(std::shared_ptr<Database> database, std::shared_ptr<SentimentAnalyzer> analyzer, const int &clientSocket);
    void start();

private:
    void handleRequest(const std::string &request);

    void handleCloseConnection(std::stringstream &receivedMessageStream);
    void handleLoginUser(std::stringstream &receivedMessageStream);
    void handleAddUser(std::stringstream &receivedMessageStream);
    void handleDeleteUser(std::stringstream &receivedMessageStream);
    void handleGetAllUsers(std::stringstream &receivedMessageStream);
    void handleAddMenuItem(std::stringstream &receivedMessageStream);
    void handleDeleteItem(std::stringstream &receivedMessageStream);
    void handleGetAllMenuItems(std::stringstream &);
    void handleGetRecommendedMenu(std::stringstream &receivedMessageStream);
    void handleGetPendingNotifications(std::stringstream &receivedMessageStream);
    void handleRollOutDailyMenu(std::stringstream &receivedMessageStream);
    void handleGenerateReport(std::stringstream &);
    void handleGetResponseOrders(std::stringstream &);
    void handleGetDailyMenu(std::stringstream &);
    void handleOrderFood(std::stringstream &receivedMessageStream);
    void handleGetTodaysOrder(std::stringstream &receivedMessageStream);
    void handleLikeDislike(std::stringstream &receivedMessageStream);
    void handleGiveFeedback(std::stringstream &receivedMessageStream);
    void handleGetDiscardedMenuItems(std::stringstream &receivedMessageStream);
    void handleRequestDiscardedItemFeedback(std::stringstream &receivedMessageStream);
    void handleGetImprovementItems(std::stringstream &receivedMessageStream);
    void handleGiveImprovmentFeedback(std::stringstream &receivedMessageStream);
    void handleGetImprovmentFeedback(std::stringstream &receivedMessageStream);
    void handleUpdateFoodPreference(std::stringstream &receivedMessageStream);

    bool updateRecommendRating(const std::string &itemId);
    std::string getCurrentDate();
};
