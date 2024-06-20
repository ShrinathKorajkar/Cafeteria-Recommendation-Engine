#include "employee.h"
#include "exception.h"
#include <sstream>

std::vector<std::string> Employee::getPendingNotifications()
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

std::vector<MenuItem> Employee::viewAllMenuItems() const
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
        throw BadActionException("Error fetching all menu items: " + std::string(e.what()));
    }
}

std::vector<MenuItem> Employee::getDailyMenu() const
{
    try
    {
        std::string request = "GET_DAILY_MENU";
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
        throw BadActionException("Error fetching daily menu: " + std::string(e.what()));
    }
}

bool Employee::orderFood(const std::vector<MenuItem> &foodItems) const
{
    try
    {
        std::string request = "ORDER_FOOD," + getId();

        for (const auto &item : foodItems)
        {
            request += "," + item.getItemId();
        }

        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);

        std::string responseStatus;
        std::getline(responseStream, responseStatus, ',');
        return responseStatus == "SUCCESS";
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error ordering food: " + std::string(e.what()));
    }

    return true;
}

std::vector<OrderItem> Employee::getTodaysOrder() const
{
    try
    {
        std::string request = "GET_TODAYS_ORDER," + getId();
        connection->send(request);

        std::string response = connection->receive();
        std::stringstream responseStream(response);
        std::vector<OrderItem> orderItems;

        std::string responseStatus;
        std::getline(responseStream, responseStatus, ',');

        if (responseStatus == "SUCCESS")
        {
            std::string itemId;
            std::string name;
            while (std::getline(responseStream, itemId, ','))
            {
                std::getline(responseStream, name, ',');
                orderItems.emplace_back(OrderItem(itemId, name));
            }
        }

        return orderItems;
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error giving Order: " + std::string(e.what()));
    }
}

bool Employee::likeDislikeItem(std::string itemId, const LikeStatus &likeStatus)
{
    try
    {
        std::string request = "LIKE_DISLIKE," + itemId + "," + likeStatusToString(likeStatus);
        connection->send(request);

        std::string response = connection->receive();
        return response == "SUCCESS";
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error giving feedback: " + std::string(e.what()));
    }
}

bool Employee::giveFeedback(const std::string &itemId, const std::string &comment) const
{
    try
    {
        std::string request = "GIVE_FEEDBACK," + itemId + "," + getId() + "," + comment;
        connection->send(request);

        std::string response = connection->receive();
        return response == "SUCCESS";
    }
    catch (const std::exception &e)
    {
        throw BadActionException("Error giving feedback: " + std::string(e.what()));
    }
}
