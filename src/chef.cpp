#include "chef.h"
#include "tcpSocketClient.h"
#include "exception.h"
#include "reportGenerator.h"

#include <sstream>

std::vector<std::string> Chef::getPendingNotifications()
{
    try
    {
        std::string request = "GET_PENDING_NOTIFICATIONS," + getId() + "," + std::to_string(getnotificationNumber());
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::vector<std::string> notifications;
        std::string responseStatus;

        std::getline(responseStream, responseStatus, ',');
        if (responseStatus == "SUCCESS")
        {
            std::string notificationData;
            std::getline(responseStream, notificationData, ',');

            notificationNumber = std::stoi(notificationData);

            while (std::getline(responseStream, notificationData, ','))
            {
                notifications.push_back(notificationData);
            }
        }

        return notifications;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching notifications: " + std::string(e.what()));
    }
}

bool Chef::rollOutDailyMenu(const std::vector<MenuItem> &items)
{
    try
    {
        std::string request = "ROLL_OUT_DAILY_MENU," + std::to_string(items.size());

        for (const auto &item : items)
        {
            request += "," + item.getItemId();
        }

        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::vector<std::string> notifications;
        std::string responseStatus;

        std::getline(responseStream, responseStatus, ',');
        if (responseStatus == "SUCCESS")
        {
            return true;
        }

        return false;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching notifications: " + std::string(e.what()));
    }
}

std::vector<OrderResponse> Chef::getOrderResponses() const
{
    try
    {
        std::string request = "GET_RESPONSE_ORDERS";
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);
        std::vector<OrderResponse> orders;
        std::string status;

        std::getline(responseStream, status, ',');
        if (status == "SUCCESS")
        {
            std::string menuItem;
            std::string quantityStr;
            int quantity;

            while (std::getline(responseStream, menuItem, ','))
            {
                std::getline(responseStream, quantityStr, ',');
                quantity = std::stoi(quantityStr);

                orders.emplace_back(menuItem, quantity);
            }
        }

        return orders;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error Generating Notification: " + std::string(e.what()));
    }
}

std::vector<MenuItem> Chef::getAllMenuItems() const
{
    try
    {
        std::string request = "GET_ALL_MENU_ITEMS";
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);
        std::vector<MenuItem> menuItems;

        std::string responseStatus;
        std::getline(responseStream, responseStatus, ',');

        if (responseStatus == "SUCCESS")
        {
            int rowCount;

            std::string tempToken;
            std::getline(responseStream, tempToken, ',');
            rowCount = std::stoi(tempToken);

            for (int i = 0; i < rowCount; i++)
            {
                std::string itemId;
                std::string name;
                double price;
                std::string description;
                FoodCategory category;
                bool availability;
                int likes;
                int dislikes;

                std::getline(responseStream, itemId, ',');
                std::getline(responseStream, name, ',');

                std::string priceStr;
                std::getline(responseStream, priceStr, ',');
                price = std::stod(priceStr);

                std::getline(responseStream, description, ',');

                std::string categoryStr;
                std::getline(responseStream, categoryStr, ',');
                category = stringToFoodCategory(categoryStr);

                std::string availabilityStr;
                std::getline(responseStream, availabilityStr, ',');
                availability = std::stoi(availabilityStr);

                std::string likesStr;
                std::getline(responseStream, likesStr, ',');
                likes = std::stoi(likesStr);

                std::string dislikesStr;
                std::getline(responseStream, dislikesStr, ',');
                dislikes = std::stoi(dislikesStr);

                std::string rating;
                std::getline(responseStream, rating, ',');

                std::getline(responseStream, tempToken, ',');
                int sentimentCount = std::stoi(tempToken);

                std::vector<std::string> sentiments;
                std::string sentiment;
                for (int j = 0; j < sentimentCount; j++)
                {
                    std::getline(responseStream, sentiment, ',');
                    sentiments.push_back(sentiment);
                }

                std::getline(responseStream, tempToken, ',');
                int commentCount = std::stoi(tempToken);
                std::vector<Comment> comments;
                for (int j = 0; j < commentCount; j++)
                {
                    Comment comment;
                    std::getline(responseStream, comment.userName, ',');
                    std::getline(responseStream, comment.commentMessage, ',');
                    std::getline(responseStream, comment.commentDate, ',');
                    comments.push_back(comment);
                }

                menuItems.emplace_back(MenuItem(itemId, name, price, description, category, availability, likes, dislikes, sentiments, comments));
            }
        }

        return menuItems;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching all menu items: " + std::string(e.what()));
    }
}

std::vector<MenuItem> Chef::getRecommendedDailyMenu() const
{
    try
    {
        std::string request = "GET_RECOMMENDED_MENU";
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);
        std::vector<MenuItem> menuItems;

        std::string responseStatus;
        std::getline(responseStream, responseStatus, ',');

        if (responseStatus == "SUCCESS")
        {
            int rowCount;

            std::string tempToken;
            std::getline(responseStream, tempToken, ',');
            rowCount = std::stoi(tempToken);

            for (int i = 0; i < rowCount; i++)
            {
                std::string itemId;
                std::string name;
                double price;
                std::string description;
                FoodCategory category;
                bool availability;
                int likes;
                int dislikes;

                std::getline(responseStream, itemId, ',');
                std::getline(responseStream, name, ',');

                std::string priceStr;
                std::getline(responseStream, priceStr, ',');
                price = std::stod(priceStr);

                std::getline(responseStream, description, ',');

                std::string categoryStr;
                std::getline(responseStream, categoryStr, ',');
                category = stringToFoodCategory(categoryStr);

                std::string availabilityStr;
                std::getline(responseStream, availabilityStr, ',');
                availability = std::stoi(availabilityStr);

                std::string likesStr;
                std::getline(responseStream, likesStr, ',');
                likes = std::stoi(likesStr);

                std::string dislikesStr;
                std::getline(responseStream, dislikesStr, ',');
                dislikes = std::stoi(dislikesStr);

                std::string ratingStr;
                std::getline(responseStream, ratingStr, ',');

                std::getline(responseStream, tempToken, ',');
                int sentimentCount = std::stoi(tempToken);

                std::vector<std::string> sentiments;
                std::string sentiment;
                for (int j = 0; j < sentimentCount; j++)
                {
                    std::getline(responseStream, sentiment, ',');
                    sentiments.push_back(sentiment);
                }

                std::getline(responseStream, tempToken, ',');
                int commentCount = std::stoi(tempToken);
                std::vector<Comment> comments;
                for (int j = 0; j < commentCount; j++)
                {
                    Comment comment;
                    std::getline(responseStream, comment.userName, ',');
                    std::getline(responseStream, comment.commentMessage, ',');
                    std::getline(responseStream, comment.commentDate, ',');
                    comments.push_back(comment);
                }

                menuItems.emplace_back(MenuItem(itemId, name, price, description, category, availability, likes, dislikes, sentiments, comments));
            }
        }

        return menuItems;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error fetching menu items: " + std::string(e.what()));
    }
}

bool Chef::updateDailyMenu(const std::vector<MenuItem> &items)
{
    // std::ostringstream oss;
    // oss << "UPDATE_DAILY_MENU," << getId();
    // for (const auto &item : items)
    // {
    //     oss << "," << item.toString();
    // }
    // connection->send(oss.str());

    std::string response = connection->receive();
    return response == "SUCCESS";
}

std::string Chef::generateReport(const int &month, const int &year) const
{
    try
    {
        std::string request = "GENERATE_REPORT," + std::to_string(month) + "," + std::to_string(year);
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);
        std::string status;

        std::getline(responseStream, status, ',');
        if (status == "SUCCESS")
        {
            std::vector<ReportData> report;

            std::string itemName;
            std::string orderCountString;
            int orderCount;

            while (std::getline(responseStream, itemName, ','))
            {
                std::getline(responseStream, orderCountString, ',');
                orderCount = std::stoi(orderCountString);

                report.emplace_back(itemName, orderCount);
            }

            ReportGenerator reportGenerator;
            return reportGenerator.generateReport(report);
        }

        return "";
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error Generating Notification: " + std::string(e.what()));
    }
}
